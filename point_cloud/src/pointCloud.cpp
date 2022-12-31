#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/pcd_io.h>
#include <thread>
#include <cstdlib>
#include <ctime>

using namespace std;

int main ()
{
    std::srand(time(NULL));
    pcl::PointCloud<pcl::PointXYZRGB> new_cloud;
    pcl::PointXYZRGB point;

    for (float i = 0.0; i < 20.0; i+= 0.01) {
        for (float j = 0.0; j < 20.0; j+=0.01) {
          point.x = rand() % 1000;
          point.y = rand() % 300;
          point.z =  rand() % 800;
          point.r = rand() % 255;
          point.g = rand() % 255;
          point.b = rand() % 255;
          new_cloud.push_back(point);
        }
    }

    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud< pcl::PointXYZRGB>(new_cloud));

    pcl::visualization::PCLVisualizer viewer ("3D point cloud");
    viewer.setBackgroundColor(0.3, 0.3, 0.3);
    pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb(cloud);
    viewer.addPointCloud<pcl::PointXYZRGB> (cloud, rgb, "sample cloud");
    viewer.setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "sample cloud");
    viewer.addCoordinateSystem (1.0);
    viewer.initCameraParameters ();

    while (!viewer.wasStopped ()) {
        viewer.spinOnce(1); 
        std::this_thread::sleep_for(10ms);
    }

    std::cout << "→ Saving point cloud data to pcd_images/test.pcd..." << std::endl;
    pcl::io::savePCDFileASCII ("../pcd_images/test.pcd", *cloud);
    std::cerr << "→ Saved " << new_cloud.size () << " data points to pcd_images/test.pcd" << std::endl;
}