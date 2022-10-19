#include<pcl/io/pcd_io.h>
#include<pcl/point_types.h>
#include <boost/thread/thread.hpp>
#include <pcl/common/common_headers.h>
#include <pcl/features/normal_3d.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <pcl/console/parse.h>

int main (int argc, char** argv){
    pcl::PointCloud<pcl::PointXYZ>::Ptr
            cloud(new pcl::PointCloud<pcl::PointXYZ>);

    for(size_t i=0; i < cloud->points.size(); i++){
    cloud->points[i].x = 1024 * rand()/(RAND_MAX+1.0f);
    cloud->points[i].y = 1024 * rand()/(RAND_MAX+1.0f);
    cloud->points[i].z = 1024 * rand()/(RAND_MAX+1.0f);
    }

    cloud->width = 100;
    cloud->height = 100;
    cloud->is_dense = false;
    cloud->points.resize(cloud->width*cloud->height);

    // Create spherical cloud with random points
    //create_random_sphere(cloud);

    boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new
pcl::visualization::PCLVisualizer ("3D Viewer"));

    viewer->addCoordinateSystem(1,"CoordinateSystem");
    viewer->addPointCloud<pcl::PointXYZ>(cloud, "cloud");

    int a = 0;
    int b = 1;

    viewer->createViewPort (0.0,0.0,0.5,1.0,a);
    viewer->setBackgroundColor(0,0,0,a); // background color dark
    viewer->addText("sample_cloud_1", 10, 10, "right", a);
    viewer->addPointCloud(cloud,  "sample_cloud_1", a);

    viewer->createViewPort (0.5,0.0,0.5,1.0,b);
    viewer->setBackgroundColor(0.1,0.1,0.1,b); // background color light
    viewer->addText("sample_cloud_2", 10, 10, "left", b);
    viewer->addPointCloud(cloud,  "sample_cloud_2", b);

    while (!viewer->wasStopped())
    {
        viewer->spinOnce(100);
        boost::this_thread::sleep(boost::posix_time::microseconds(100000));
    }

}