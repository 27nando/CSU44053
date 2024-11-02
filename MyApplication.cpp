
#include "Utilities.h"


// Ground truth for pedestrian crossings.  Each row contains
// 1. the image number (PC?.jpg)
// 2. the coordinates of the line at the top of the pedestrian crossing (left column, left row, right column, right row)
// 3. the coordinates of the line at the bottom of the pedestrian crossing (left column, left row, right column, right row)
int pedestrian_crossing_ground_truth[][9] = {
	 { 10,0,132,503,113,0,177,503,148},
	{ 11,0,131,503,144,0,168,503,177},
	{ 12,0,154,503,164,0,206,503,213},
	{ 13,0,110,503,110,0,156,503,144},
	{ 14,0,95,503,104,0,124,503,128},
	{ 15,0,85,503,91,0,113,503,128},
	{ 16,0,65,503,173,0,79,503,215},
	{ 17,0,43,503,93,0,89,503,146},
	{ 18,0,122,503,117,0,169,503,176},
	{ 20,0,157,503,131,0,223,503,184},
	{ 21,0,140,503,136,0,190,503,183},
	{ 22,0,114,503,97,0,140,503,123},
	{ 23,0,133,503,122,0,198,503,186},
	{ 24,0,107,503,93,0,146,503,118},
	{ 25,0,58,503,164,0,71,503,204},
	{ 26,0,71,503,131,0,106,503,199},
	{ 27,0,138,503,151,0,179,503,193}
};

void MyApplication()
{
	for (int image_index = 10; (image_index <= 29); image_index++)
	{
		char filename[200];
		sprintf(filename, "Media/PedestrianCrossings/PC%d.jpg", image_index);
		Mat original_image;
		original_image = imread(filename, -1);

		if (original_image.empty()) {
			cerr << "Could not open or find the image: " << filename << endl;
			continue; // Skip to the next iteration if the image is not found
		}

		Mat gray_image, blurred_image, edge_image, morph_image;
		cvtColor(original_image, gray_image, COLOR_BGR2GRAY);
		GaussianBlur(gray_image, blurred_image, Size(5, 5), 1.5);

		Canny(blurred_image, edge_image, 100, 150);

		Mat closed_image;
		Mat five_by_five_element(5, 5, CV_8U, Scalar(1));
		morphologyEx(edge_image, closed_image, MORPH_CLOSE, five_by_five_element);

		vector<vector<Point>> contours;
		findContours(edge_image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		Mat contour_image = original_image.clone();
		vector<Rect> potential_crossings;
		for (const auto& contour : contours) {
			Rect bounding_rect = boundingRect(contour);
			float aspect_ratio = (float)bounding_rect.width / bounding_rect.height;
			double area = contourArea(contour);

			// Check for rectangular shape and reasonable area
			if (aspect_ratio > 1.5 && aspect_ratio < 5.0 && area > 500 && area < 5000) {
				potential_crossings.push_back(bounding_rect);
				rectangle(contour_image, bounding_rect, Scalar(0, 255, 0), 2); // Draw rectangles for detected regions
			}
		}

		imshow(filename, original_image);
		imshow("Edge Detection", closed_image);
		imshow("Contours", contour_image);
		char cha = cv::waitKey();
		cv::destroyAllWindows();
	}
}
