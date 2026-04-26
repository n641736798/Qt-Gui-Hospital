#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    std::cout << "OpenCV version: " << CV_VERSION << std::endl;
    std::cout << "Testing cv::imread..." << std::endl;

    cv::Mat mat = cv::imread("D:/study/C++ code/Qt-Gui-Hospital-main/test_image.png", cv::IMREAD_UNCHANGED);

    if (mat.empty()) {
        std::cout << "Image load returned empty (but no crash)" << std::endl;
    } else {
        std::cout << "Image loaded! size=" << mat.cols << "x" << mat.rows
                  << " channels=" << mat.channels()
                  << " type=" << mat.type() << std::endl;
    }

    std::cout << "Done." << std::endl;
    return 0;
}
