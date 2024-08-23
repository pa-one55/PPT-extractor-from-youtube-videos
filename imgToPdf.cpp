#include <hpdf.h>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;

// Function to add an image to the PDF
void addImageToPDF(HPDF_Doc pdf, const string& imagePath) {
    // Load image using OpenCV
    Mat image = imread(imagePath, IMREAD_UNCHANGED);
    if (image.empty()) {
        cerr << "Error: Could not open or find the image: " << imagePath << endl;
        return;
    }

    // Create a new page with A4 landscape size
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_LANDSCAPE);

    // Get the dimensions of the page
    float pageWidth = HPDF_Page_GetWidth(page);
    float pageHeight = HPDF_Page_GetHeight(page);

    // Create a new image object in the PDF
    HPDF_Image pdfImage = HPDF_LoadPngImageFromFile(pdf, imagePath.c_str());

    // Calculate the image's aspect ratio
    float imageWidth = HPDF_Image_GetWidth(pdfImage);
    float imageHeight = HPDF_Image_GetHeight(pdfImage);
    float aspectRatio = imageWidth / imageHeight;

    // Scale image to fit the page while maintaining aspect ratio
    float scaleX = pageWidth / imageWidth;
    float scaleY = pageHeight / imageHeight;
    float scale = min(scaleX, scaleY);

    // Center the image on the page
    float offsetX = (pageWidth - (imageWidth * scale)) / 2;
    float offsetY = (pageHeight - (imageHeight * scale)) / 2;

    // Draw the image on the page
    HPDF_Page_DrawImage(page, pdfImage, offsetX, offsetY, imageWidth * scale, imageHeight * scale);
}

int main() {
    // Create a new PDF document
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        cerr << "Error: Unable to create PDF document." << endl;
        return 1;
    }

    // Directory containing images
    string imageDir = "/home/paone/frame/filtered_frames"; // Replace with your image directory path

    // Check if the directory exists
    if (!fs::exists(imageDir) || !fs::is_directory(imageDir)) {
        cerr << "Error: The directory does not exist or is not a directory: " << imageDir << endl;
        return 1;
    }

    // Vector to hold the image paths
    vector<fs::path> imagePaths;

    // Iterate over all PNG files in the specified directory and store their paths
    for (const auto& entry : fs::directory_iterator(imageDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            imagePaths.push_back(entry.path());
        }
    }

    // Sort the image paths by filename
    sort(imagePaths.begin(), imagePaths.end());

    // Add each image to the PDF
    for (const auto& path : imagePaths) {
        addImageToPDF(pdf, path.string());
    }

    // Save the PDF document to a file
    HPDF_SaveToFile(pdf, "output.pdf");

    // Clean up and free resources
    HPDF_Free(pdf);

    return 0;
}
