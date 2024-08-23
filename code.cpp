#include <opencv2/core.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

using namespace cv;
using namespace std;

/*
This function opens a video file and extracts the frames and puts them into a vector of Mat (class for representing an image).
*/
void extract_frames(const string &videoFilePath, vector<Mat>& frames) {
    try {
        // Open the video file
        VideoCapture cap(videoFilePath); // open the video file
        if (!cap.isOpened()) {  // check if we succeeded
            throw runtime_error("Cannot open video file");
        }

        // Get the total number of frames in the video
        int frameCount = static_cast<int>(cap.get(CAP_PROP_FRAME_COUNT));

        // Extract frames
        for (int frameNum = 0; frameNum < frameCount; ++frameNum) {
            Mat frame;
            cap >> frame; // get the next frame from video
            if (frame.empty()) { // Break the loop if no more frames
                break;
            }
            frames.push_back(frame);
        }
    }
    catch (const cv::Exception& e) {
        cerr << "OpenCV Exception: " << e.what() << endl;
        exit(1);
    }
    catch (const std::exception& e) {
        cerr << "Standard Exception: " << e.what() << endl;
        exit(1);
    }
}

/*
It saves a vector of frames into jpg images in the outputDir as 1.jpg, 2.jpg, etc., where 1, 2, etc., represents the frame number.
*/
void save_frames(const vector<Mat>& frames, const string& outputDir) {
    vector<int> compression_params;
    compression_params.push_back(IMWRITE_JPEG_QUALITY);
    compression_params.push_back(100);
  
    for (size_t frameNumber = 0; frameNumber < frames.size(); ++frameNumber) {
        string filePath = outputDir + to_string(frameNumber + 1) + ".jpg"; // Frame numbers start from 1
        if (!imwrite(filePath, frames[frameNumber], compression_params)) {
            cerr << "Failed to save image: " << filePath << endl;
        }
    }
}

// Example of calling the functions
// int main() {
//     vector<Mat> frames;
//     extract_frames("home\paone\frame\Cache.mp4", frames);
//     save_frames(frames, "home\paone\frame\frames_extracted");
//     return 0;
// }


// int main() {
//     vector<Mat> frames;
//     extract_frames("/home/paone/frame/Cache.mp4", frames);
//     save_frames(frames, "/home/paone/frame/frames_extracted/");
//     return 0;
// }


int main() {
    VideoCapture cap("/home/paone/frame/Cache.mp4");
    if (!cap.isOpened()) {
        cerr << "Error: Could not open the video file." << endl;
        return 1;
    }
    
    Mat frame;
    cap >> frame;
    if (!frame.empty()) {
        cout << "Frame read successfully." << endl;
    } else {
        cout << "Failed to read a frame." << endl;
    }
    
    return 0;
}
