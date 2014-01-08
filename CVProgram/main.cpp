#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char *argv[])
{
	cv::Mat src_img = cv::imread("building.jpg", 1);
	if (src_img.empty()) return -1;

	cv::Mat dst_img, work_img;
	dst_img = src_img.clone();
	cv::cvtColor(src_img, work_img, CV_BGR2GRAY);
	cv::Canny(work_img, work_img, 50, 200, 3);

	std::vector<cv::Vec2f> lines;
	cv::HoughLines(work_img, lines, 1, CV_PI / 180, 200, 0, 0);

	std::vector<cv::Vec2f>::iterator it = lines.begin();
	for (; it != lines.end(); ++it) {
		float rho = (*it)[0], theta = (*it)[1];
		cv::Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cv::saturate_cast<int>(x0 + 1000 * (-b));
		pt1.y = cv::saturate_cast<int>(y0 + 1000 * (a));
		pt2.x = cv::saturate_cast<int>(x0 - 1000 * (-b));
		pt2.y = cv::saturate_cast<int>(y0 - 1000 * (a));
		cv::line(dst_img, pt1, pt2, cv::Scalar(0, 0, 255), 3, CV_AA);
	}

	cv::namedWindow("HoughLines", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
	cv::imshow("HoughLines", dst_img);
	cv::waitKey(0);
}