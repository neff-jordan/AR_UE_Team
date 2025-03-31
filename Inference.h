// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"


// Undefine Apple's 'check' macro to prevent conflicts
#ifdef check
#undef check
#endif

#include <opencv2/core.hpp> // might be <> instead of "" idk
#include <opencv2/opencv_modules.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgcodecs.hpp>

// Cpp native
#include <fstream>
#include <vector>
#include <string>
#include <random>

#include "Inference.generated.h"


/**
 * A struct to hold the detection results for objects found in an image.
 *
 * Unreal cannot process certain openCV/cpp data types such as std::string, cv::Scalar, cv::Rect so a replacement is needed. Unreal Engine provides the following alternatives.
 * - FString            instead of std::string (cpp)
 * - FLinearColor   instead of cv::Scalar (opencv)
 * - TArray             instead of std::vector (cpp)
 *
 */
USTRUCT(BlueprintType)
struct FDetection
{
    GENERATED_BODY()
    
    //UPROPERTY(BlueprintCallable, Category = "Inference")
    int class_id{0};
    
    //UPROPERTY(BlueprintCallable, Category = "Inference")
    FString className{};
    
    //UPROPERTY(BlueprintCallable, Category = "Inference")
    float confidence{0.0};
    
    //UPROPERTY(BlueprintCallable, Category = "Inference")
    FLinearColor color; // check for channel format i.e. rgb vs bgr etc. since the default type was cv::Scalar
    
    //UPROPERTY(BlueprintCallable, Category = "Inference")
    cv::Rect box; // verify that this translates to cv::Rect
};

/**
 * 
 */
UCLASS()
class WCUCAPSTONEPROJECT_API UInference : public UObject
{
	GENERATED_BODY()
    
    public:
        //UInference();
        void Init(const FString &onnxModelPath, const cv::Size2f &modelInputShape, const FString &classesTxtFile, bool runWithCuda = true);

        TArray<FDetection> runInference(const cv::Mat &input);

    private:
        void loadClassesFromFile();
        void loadOnnxNetwork();
        cv::Mat formatToSquare(const cv::Mat &source);

        FString modelPath{};
        FString classesPath{};
        bool cudaEnabled{};

        TArray<FString> classes{};
        cv::Size2f modelShape{};

        float modelConfidenseThreshold {0.25};
        float modelScoreThreshold      {0.45};
        float modelNMSThreshold        {0.50};

        bool letterBoxForSquare = true;

        cv::dnn::Net net;
};
