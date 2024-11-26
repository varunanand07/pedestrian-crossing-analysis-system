#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>

using namespace cv;
using namespace std;

int pedestrian_crossing_ground_truth[][9] = {
    {10, 0, 132, 503, 113, 0, 177, 503, 148},
    {11, 0, 131, 503, 144, 0, 168, 503, 177},
    {12, 0, 154, 503, 164, 0, 206, 503, 213},
    {13, 0, 110, 503, 110, 0, 156, 503, 144},
    {14, 0, 95, 503, 104, 0, 124, 503, 128},
    {15, 0, 85, 503, 91, 0, 113, 503, 128},
    {16, 0, 65, 503, 173, 0, 79, 503, 215},
    {17, 0, 43, 503, 93, 0, 89, 503, 146},
    {18, 0, 122, 503, 117, 0, 169, 503, 176},
    {20, 0, 157, 503, 131, 0, 223, 503, 184},
    {21, 0, 140, 503, 136, 0, 190, 503, 183},
    {22, 0, 114, 503, 97, 0, 140, 503, 123},
    {23, 0, 133, 503, 122, 0, 198, 503, 186},
    {24, 0, 107, 503, 93, 0, 146, 503, 118},
    {25, 0, 58, 503, 164, 0, 71, 503, 204},
    {26, 0, 71, 503, 131, 0, 106, 503, 199},
    {27, 0, 138, 503, 151, 0, 179, 503, 193}
};

bool get_ground_truth(int image_index, Vec4i &top_line, Vec4i &bottom_line)
{
    int num_ground_truth = sizeof(pedestrian_crossing_ground_truth) / sizeof(pedestrian_crossing_ground_truth[0]);
    for (int i = 0; i < num_ground_truth; i++)
    {
        if (pedestrian_crossing_ground_truth[i][0] == image_index)
        {
            top_line = Vec4i(pedestrian_crossing_ground_truth[i][1], pedestrian_crossing_ground_truth[i][2],
                             pedestrian_crossing_ground_truth[i][3], pedestrian_crossing_ground_truth[i][4]);
            bottom_line = Vec4i(pedestrian_crossing_ground_truth[i][5], pedestrian_crossing_ground_truth[i][6],
                                pedestrian_crossing_ground_truth[i][7], pedestrian_crossing_ground_truth[i][8]);
            return true;
        }
    }
    return false;
}

void MyApplication()
{
    int true_positives = 0;
    int false_positives = 0;
    int false_negatives = 0;

    for (int image_index = 10; image_index <= 29; image_index++)  
    {
        std::string filename = "../Media/PedestrianCrossings/PC" + std::to_string(image_index) + ".jpg";
        Mat original_image = imread(filename, -1);
        if (original_image.empty())
        {
            cout << "Could not open or find the image: " << filename << endl;
            continue;
        }

        Mat gray_image;
        cvtColor(original_image, gray_image, COLOR_BGR2GRAY);
        GaussianBlur(gray_image, gray_image, Size(5, 5), 0);

        Mat edges;
        Canny(gray_image, edges, 50, 150, 3);

        vector<Vec4i> lines;
        HoughLinesP(edges, lines, 1, CV_PI / 180, 50, 50, 10);

        vector<Vec4i> horizontal_lines;
        vector<int> y_coords;
        for (size_t i = 0; i < lines.size(); i++)
        {
            Vec4i l = lines[i];
            double angle = atan2(l[3] - l[1], l[2] - l[0]) * 180 / CV_PI;
            if (abs(angle) < 10) 
            {
                horizontal_lines.push_back(l);
                y_coords.push_back(l[1]);
                y_coords.push_back(l[3]);
            }
        }

        if (y_coords.size() == 0)
        {
            cout << "No horizontal lines detected in image " << image_index << endl;
            continue;
        }

        sort(y_coords.begin(), y_coords.end());
        int detected_top_y = y_coords.front();
        int detected_bottom_y = y_coords.back();
        int detected_vertical_range = detected_bottom_y - detected_top_y;

        Vec4i gt_top_line, gt_bottom_line;
        bool has_ground_truth = get_ground_truth(image_index, gt_top_line, gt_bottom_line);

        int gt_top_y = (gt_top_line[1] + gt_top_line[3]) / 2;
        int gt_bottom_y = (gt_bottom_line[1] + gt_bottom_line[3]) / 2;
        int gt_vertical_range = abs(gt_bottom_y - gt_top_y);

        int overlap_top = max(detected_top_y, gt_top_y);
        int overlap_bottom = min(detected_bottom_y, gt_bottom_y);
        int overlap = overlap_bottom - overlap_top;

        double overlap_ratio = 0;
        if (gt_vertical_range > 0)
            overlap_ratio = (double)overlap / gt_vertical_range;

        bool pedestrian_crossing_detected = overlap_ratio > 0.5;

        if (has_ground_truth)
        {
            if (pedestrian_crossing_detected)
            {
                true_positives++;
            }
            else
            {
                false_negatives++;
            }
        }
        else if (pedestrian_crossing_detected)
        {
            false_positives++;
        }

        Mat result_image = original_image.clone();

        if (has_ground_truth)
        {
            line(result_image, Point(gt_top_line[0], gt_top_line[1]), Point(gt_top_line[2], gt_top_line[3]), Scalar(0, 255, 0), 2, LINE_AA);
            line(result_image, Point(gt_bottom_line[0], gt_bottom_line[1]), Point(gt_bottom_line[2], gt_bottom_line[3]), Scalar(0, 255, 0), 2, LINE_AA);
        }

        if (pedestrian_crossing_detected)
        {
            putText(result_image, "Pedestrian Crossing Detected", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
        }
        else
        {
            putText(result_image, "No Pedestrian Crossing Detected", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
        }

        imshow("Result", result_image);
        char cha = cv::waitKey(0);
        cv::destroyAllWindows();
    }

    cout << "Performance evaluation:" << endl;
    cout << "True Positives: " << true_positives << endl;
    cout << "False Positives: " << false_positives << endl;
    cout << "False Negatives: " << false_negatives << endl;

    double precision = (true_positives + false_positives) > 0 ? (double)true_positives / (true_positives + false_positives) : 0;
    double recall = (true_positives + false_negatives) > 0 ? (double)true_positives / (true_positives + false_negatives) : 0;

    cout << "Precision: " << precision << endl;
    cout << "Recall: " << recall << endl;
}

int main()
{
    MyApplication();
    return 0;
}
