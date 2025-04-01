// Fill out your copyright notice in the Description page of Project Settings.


#include "MainOpenCV.h"
#include "Inference.h"


using namespace std;
using namespace cv;

// int argc, char **argv
int UMainOpenCV::Start() // ATTACH blueprint photo path
{
    // i dont think i need this cause i am not running on CLI
    //FString projectBasePath = "/home/.../yolov8_CPP_Inference_OpenCV_ONNX";
    
    bool runOnGPU = false;

    //
    // Pass in either:
    //
    // "/source/models/yolov8s.onnx"
    // or
    // "/source/models/yolov5s.onnx"
    //
    // To run Inference with yolov8/yolov5 (ONNX)
    //
    /*
     Unreal cannot process certain openCV/cpp data types such as std::string, cv::Scalar, cv::Rect so a replacement is needed. Unreal Engine provides the following alternatives.
     * - FString            instead of std::string (cpp)
     * - FLinearColor       instead of cv::Scalar (opencv)
     * - FIntRect           instead of cv::Rect (opencv)
     * - TArray             instead of std::vector (cpp)
     */
    FString modelPath = FPaths::ProjectSavedDir() / TEXT("yolov8s.onnx");
    if (!FPaths::FileExists(modelPath))
    {
        UE_LOG(LogTemp, Error, TEXT("ONNX Model not found: %s"), *modelPath);
        return -1; // Prevent crash
    }
    UInference* inf = NewObject<UInference>();
    inf->Init(FPaths::ProjectSavedDir() / TEXT("yolov8s.onnx"), cv::Size(640, 480),
              FPaths::ProjectSavedDir() / TEXT("classes.txt"), runOnGPU);

    // change this to file path where UE screenshots are stored
    TArray<FString> imageNames;
    imageNames.Add(FPaths::ProjectSavedDir() / TEXT("bus.jpg"));
    imageNames.Add("/Users/jordanneff/Documents/Unreal Projects/WCUCapstoneProject/saved/zidane.jpg");
    
    //imageNames.Add("/Users/jordanneff/Documents/Unreal Projects/WCUCapstoneProject/saved/bike.jpg");
    //imageNames.Add(FPaths::ProjectSavedDir() / TEXT("laptop.jpg"));
    //imageNames.Add(FPaths::ProjectSavedDir() / TEXT("sport_ball.jpg"));
    //imageNames.Add(FPaths::ProjectSavedDir() / TEXT("tennis.jpeg"));
    imageNames.Add(FPaths::ProjectSavedDir() / TEXT("wine.jpg"));
    imageNames.Add(FPaths::ProjectSavedDir() / TEXT("cow.jpg"));
    imageNames.Add(FPaths::ProjectSavedDir() / TEXT("bench.jpg"));
    imageNames.Add(FPaths::ProjectSavedDir() / TEXT("bikers.jpg"));
    imageNames.Add(FPaths::ProjectSavedDir() / TEXT("umbrella.jpg"));


    for (int i = 0; i < imageNames.Num(); ++i)
    {
        //
        UE_LOG(LogTemp, Warning, TEXT("Attempting to load image: %s"), *imageNames[i]);
        cv::Mat frame = cv::imread(TCHAR_TO_UTF8(*imageNames[i]));
        if (frame.empty())
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load image: %s"), *imageNames[i]);
            continue;
        }
        //
        
        UE_LOG(LogTemp, Warning, TEXT("Image [%s] - Channels: %d, Depth: %d"), *imageNames[i], frame.channels(), frame.depth());


        // Inference starts here...
        TArray<FDetection> output = inf->runInference(frame);

        int detections = output.Num();
        UE_LOG(LogTemp, Warning, TEXT("Number of detections: %d"), detections);

        for (int i = 0; i < detections; ++i)
        {
            FDetection detection = output[i];

            FString classNameString = detection.className;
            FString confidenceString = FString::Printf(TEXT("%.4f"), detection.confidence);
            
            // Log the output to verify values
            UE_LOG(LogTemp, Warning, TEXT("Detection %d: Class: %s, Confidence: %s"),
                       i, *classNameString, *confidenceString);
            // need one for bounding boxes
        }
    }
    return 0;
}
