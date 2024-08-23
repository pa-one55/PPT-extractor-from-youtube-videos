#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace cv;
using namespace std;

int main() {
    string url;
    string videoFileName = "downloaded_video.mp4"; // Video file name after downloading

    // Step 1: Read the URL from the input file
    ifstream inputFile("video_url.txt");
    if (!inputFile) {
        cerr << "Failed to open input file." << endl;
        return -1;
    }

    getline(inputFile, url);
    inputFile.close();

    // Step 2: Download the video using yt-dlp
    string command = "yt-dlp -o " + videoFileName + " " + url;
    if (system(command.c_str()) != 0) {
        cerr << "Failed to download the video." << endl;
        return -1;
    }

    // Step 3: Open the video using OpenCV
    VideoCapture cap(videoFileName);
    if (!cap.isOpened()) {
        cerr << "Error opening video file." << endl;
        return -1;
    }

    int frameCount = 0;
    Mat frame;
    string outputDir = "frames/";

    // Ensure output directory exists
    string createDirCommand = "mkdir -p " + outputDir;
    system(createDirCommand.c_str());

    // Step 4: Extract frames
    while (cap.read(frame)) {
        string frameFileName = outputDir + "frame_" + to_string(frameCount) + ".png";
        imwrite(frameFileName, frame);
        frameCount++;
    }

    cout << "Extracted " << frameCount << " frames from the video." << endl;

    return 0;
}
