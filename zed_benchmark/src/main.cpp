#include <analysis.hpp>

#include <sl/Camera.hpp>
#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <limits>
#include <string>
#include <algorithm>
#include <chrono>

#include <stdlib.h>
#include <signal.h>

cv::Mat slMat2cvMat(sl::Mat& input);

std::atomic<bool> exit_flag(false);
void exit_hl(int signum) {
    exit_flag = true;
}

int main(int argc, char **argv) {
    // Create a ZED camera object
    sl::Camera zed;

    sl::PositionalTrackingParameters tracking_parameters;
    tracking_parameters.set_as_static = true;

    // Set configuration parameters
    sl::InitParameters init_parameters;
    init_parameters.camera_disable_self_calib = true;
    init_parameters.camera_resolution = sl::RESOLUTION::HD2K; 
    init_parameters.camera_fps = 15;
    init_parameters.depth_mode = sl::DEPTH_MODE::PERFORMANCE;
    init_parameters.coordinate_units = sl::UNIT::MILLIMETER;
    init_parameters.depth_stabilization = true;

    if (argc > 1)
    {
        init_parameters.input.setFromSVOFile(argv[1]);
    }

    sl::RuntimeParameters runtime_parameters;
    runtime_parameters.sensing_mode = sl::SENSING_MODE::STANDARD;

    // Open the camera
    auto returned_state = zed.open(init_parameters);
    if (returned_state != sl::ERROR_CODE::SUCCESS) {
        std::cout << "Error " << returned_state 
            << ", exit program." << std::endl;
        return EXIT_FAILURE;
    }
    returned_state = zed.enablePositionalTracking(tracking_parameters);
    if (returned_state != sl::ERROR_CODE::SUCCESS) {
        std::cout << "Error " << returned_state 
            << ", exit program." << std::endl;
        return EXIT_FAILURE;
    }

    sl::Resolution image_shape = zed.getCameraInformation()
        .camera_configuration.resolution;
    sl::Mat image(image_shape.width, image_shape.height, sl::MAT_TYPE::U8_C4);
    cv::Mat image_cv = slMat2cvMat(image);
    sl::Mat depth(image_shape.width, image_shape.height, sl::MAT_TYPE::F32_C1); //< With retrieveMeasure
    // sl::Mat depth(image_shape.width, image_shape.height, sl::MAT_TYPE::U16_C1); //< With retrieveMeasure
    // sl::Mat depth(image_shape.width, image_shape.height, sl::MAT_TYPE::U8_C4); // With retrieveImage.
    cv::Mat depth_cv = slMat2cvMat(depth);

    signal(SIGINT, exit_hl);
    int i = 0;
    int it;
    std::cout << "→ Enter the number of iterations: ";
    std::cin >> it;

    parco::analysis::TimeVector<double> _exec;
    while (!exit_flag && i < it) {
        // Grab an image
        returned_state = zed.grab(runtime_parameters);
        // A new image is available if grab() returns ERROR_CODE::SUCCESS
        if (returned_state == sl::ERROR_CODE::SUCCESS) {
            // Get the left image
            zed.retrieveImage(image, sl::VIEW::LEFT, sl::MEM::CPU);
            _exec.start();
            zed.retrieveMeasure(depth, sl::MEASURE::DEPTH, sl::MEM::CPU); //< With retrieveMeasure.
            _exec.stop();
            // zed.retrieveMeasure(depth, sl::MEASURE::DEPTH_U16_MM, sl::MEM::CPU); //< With retrieveMeasure.
            // zed.retrieveImage(depth, sl::VIEW::DEPTH, sl::MEM::CPU); //< With retrieveImage.
            
            int svo_position = zed.getSVOPosition();
            std::cout << "SVO pos: " << svo_position
                << " || CurrFPS: " << zed.getCurrentFPS() 
                    << " (" << zed.getCameraInformation().camera_fps << ")"
                << std::endl;

            cv::Mat image_copy;
            cv::resize(image_cv, image_copy, 
                cv::Size(image_cv.cols / 2, image_cv.rows / 2));
            cv::Mat depth_copy;
            cv::resize(depth_cv, depth_copy, 
                cv::Size(depth_cv.cols / 2, depth_cv.rows / 2));

            cv::imshow("color", image_copy);
            cv::imshow("depth", depth_copy);

            // if (svo_position >= 100) exit_flag = true;
            if (cv::waitKey(1) == 'q') exit_flag = true;
            i++;
        }
    }
    std::cout << std::endl << "→ Number of iteration: " << i << std::endl;

    parco::analysis::Matrix<double> matrix_analysis(

            {{"gpu_exec", _exec.values()}});

    matrix_analysis.show_analysis();

    matrix_analysis.dump_analysis();

    // Close the camera
    zed.close();
    return EXIT_SUCCESS;
}

int getOCVtype(sl::MAT_TYPE type) {
    int cv_type = -1;
    switch (type) {
        case sl::MAT_TYPE::F32_C1: cv_type = CV_32FC1; break;
        case sl::MAT_TYPE::F32_C2: cv_type = CV_32FC2; break;
        case sl::MAT_TYPE::F32_C3: cv_type = CV_32FC3; break;
        case sl::MAT_TYPE::F32_C4: cv_type = CV_32FC4; break;
        case sl::MAT_TYPE::U8_C1: cv_type = CV_8UC1; break;
        case sl::MAT_TYPE::U8_C2: cv_type = CV_8UC2; break;
        case sl::MAT_TYPE::U8_C3: cv_type = CV_8UC3; break;
        case sl::MAT_TYPE::U8_C4: cv_type = CV_8UC4; break;
        case sl::MAT_TYPE::U16_C1: cv_type = CV_16UC1; break;
        default: break;
    }
    return cv_type;
}

cv::Mat slMat2cvMat(sl::Mat& input) {
    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), 
        getOCVtype(input.getDataType()), 
        input.getPtr<sl::uchar1>(sl::MEM::CPU), 
        input.getStepBytes(sl::MEM::CPU));
}

cv::cuda::GpuMat slMat2cvMatGPU(sl::Mat& input) {
    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    return cv::cuda::GpuMat(input.getHeight(), input.getWidth(), 
        getOCVtype(input.getDataType()), 
        input.getPtr<sl::uchar1>(sl::MEM::GPU), 
        input.getStepBytes(sl::MEM::GPU));
}
