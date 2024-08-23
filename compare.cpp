#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <iomanip> // For zero-padding

using namespace cv;
using namespace std;
namespace fs = std::filesystem;

// Compare two images and return whether they are different based on a threshold
bool areImagesDifferent(const Mat& img1, const Mat& img2, double threshold = 0.40) {
    if (img1.size() != img2.size()) return true; // Images of different sizes are considered different

    Mat diff;
    absdiff(img1, img2, diff);

    // Convert difference to grayscale for calculating difference percentage
    Mat grayDiff;
    cvtColor(diff, grayDiff, COLOR_BGR2GRAY);

    double nonZeroCount = countNonZero(grayDiff);
    double totalPixels = img1.rows * img1.cols;
    double diffPercentage = nonZeroCount / totalPixels;

    return diffPercentage > threshold;
}

// Sequentially filter images
vector<Mat> filterImagesSequentially(const vector<Mat>& images, double threshold = 0.20) {
    vector<Mat> filteredImages;

    if (images.empty()) return filteredImages;

    // Start with the first image
    filteredImages.push_back(images[0]);
    Mat currentImage = images[0];

    for (size_t i = 1; i < images.size(); ++i) {
        if (areImagesDifferent(currentImage, images[i], threshold)) {
            filteredImages.push_back(images[i]);
            currentImage = images[i]; // Update the current image
        }
    }

    return filteredImages;
}

// Load and sort image filenames from a directory
vector<string> getSortedImageFilenames(const string& directory) {
    vector<string> filenames;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".png") {
            filenames.push_back(entry.path().string());
        }
    }

    // Sort filenames (numerical sorting if filenames contain numbers)
    sort(filenames.begin(), filenames.end(), [](const string& a, const string& b) {
        return fs::path(a).filename().string() < fs::path(b).filename().string();
    });

    return filenames;
}

// Load images from sorted filenames
vector<Mat> loadImages(const vector<string>& filenames) {
    vector<Mat> images;
    for (const auto& filename : filenames) {
        Mat img = imread(filename, IMREAD_COLOR); // Read in color
        if (!img.empty()) {
            images.push_back(img);
        }
    }
    return images;
}

// Save images to a directory
void saveImages(const vector<Mat>& images, const string& outputDir) {
    for (size_t i = 0; i < images.size(); ++i) {
        ostringstream filePath;
        filePath << outputDir << "img" << setw(2) << setfill('0') << (i + 1) << ".png";
        imwrite(filePath.str(), images[i]);
    }
}

// Main function
int main() {
    string inputDir = "/home/paone/frame/frames_extracted/";
    string outputDir = "/home/paone/frame/filtered_frames/";

    // Ensure the output directory exists
    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    // Get sorted image filenames
    vector<string> filenames = getSortedImageFilenames(inputDir);

    // Load images
    vector<Mat> images = loadImages(filenames);

    // Filter and save images
    vector<Mat> filteredImages = filterImagesSequentially(images);
    saveImages(filteredImages, outputDir);

    cout << "Filtered images saved to " << outputDir << endl;

    return 0;
}
