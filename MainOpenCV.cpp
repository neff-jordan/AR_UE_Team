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
    imageNames.Add(FPaths::ProjectSavedDir() / TEXT("zidane.jpg"));

    for (int i = 0; i < imageNames.Num(); ++i)
    {
        //
        cv::Mat frame = cv::imread(TCHAR_TO_UTF8(*imageNames[i]));
        if (frame.empty())
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load image: %s"), *imageNames[i]);
            continue;
        }
        //

        // Inference starts here...
        TArray<FDetection> output = inf->runInference(frame);

        int detections = output.Num();
        // write output logs here to verify object
        
        
        
        
        //std::cout << "Number of detections:" << detections << std::endl;

        //for (int i = 0; i < detections; ++i)
        //{
            /* i dont think i need this either since that is for the GUI. i will need to draw that to unreal eventually tho.
             
            FDetection detection = output[i];
            FIntRect box = detection.box;
            FLinearColor color = detection.color;
            // Detection box
            cv::rectangle(frame, box, color, 2); // what is the FIntRect equivalent
            // Detection box text
            FString classNameString = FString(detection.className.c_str());
            FString confidenceString = FString::Printf(TEXT("%.4f"), detection.confidence);
            FString classString = classNameString + TEXT(" ") + confidenceString;
            */

            /*
             
             Orignal code.
             
                        Detection detection = output[i];

                         cv::Rect box = detection.box;
                         cv::Scalar color = detection.color;

                         // Detection box
                         cv::rectangle(frame, box, color, 2);

                         // Detection box text
                         std::string classString = detection.className + ' ' + std::to_string(detection.confidence).substr(0, 4);
                         cv::Size textSize = cv::getTextSize(classString, cv::FONT_HERSHEY_DUPLEX, 1, 2, 0);
                         cv::Rect textBox(box.x, box.y - 40, textSize.width + 10, textSize.height + 20);

                         cv::rectangle(frame, textBox, color, cv::FILLED);
                         cv::putText(frame, classString, cv::Point(box.x + 5, box.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 0, 0), 2, 0);
                     
             */
        //}
        // Inference ends here...

        
        
        // This is only for preview purposes ** i should probably delete this since i dont need it to make a GUI
        //float scale = 0.8;
        //cv::resize(frame, frame, cv::Size(frame.cols*scale, frame.rows*scale));
        //cv::imshow("Inference", frame);
        //

        //cv::waitKey(-1); // ? do i need
    }
    return 0;
}
