// Fill out your copyright notice in the Description page of Project Settings.

#include "Inference.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

//UInference::UInference() {}

void UInference::Init(const FString &onnxModelPath, const cv::Size2f &modelInputShape, const FString &classesTxtFile, bool runWithCuda)
{
    modelPath = onnxModelPath;
    modelShape = modelInputShape;
    classesPath = classesTxtFile;
    cudaEnabled = runWithCuda;

    loadOnnxNetwork();
    loadClassesFromFile();
}

TArray<FDetection> UInference::runInference(const cv::Mat &input)
{
    UE_LOG(LogTemp, Warning, TEXT("Running inference"));
    cv::Mat modelInput = input;
    
    if (letterBoxForSquare && modelShape.width == modelShape.height)
    {
        modelInput = formatToSquare(modelInput);
    }

    if (modelInput.empty())
    {
        UE_LOG(LogTemp, Error, TEXT("Model input is empty!"));
        return {};
    }

    cv::Mat blob;
    cv::dnn::blobFromImage(modelInput, blob, 1.0/255.0, modelShape, cv::Scalar(), true, false);
    net.setInput(blob);

    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    if (outputs.empty())
    {
        UE_LOG(LogTemp, Error, TEXT("ONNX model returned empty output!"));
        return {};
    }

    int rows = outputs[0].size[1];
    int dimensions = outputs[0].size[2];

    bool yolov8 = true;
    //if (dimensions > rows)
    //{
        //yolov8 = true;
        rows = outputs[0].size[2];
        dimensions = outputs[0].size[1];
        outputs[0] = outputs[0].reshape(1, dimensions);
        cv::transpose(outputs[0], outputs[0]);
    //}

    float *data = (float *)outputs[0].data;
    if (!data)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to access output data"));
        return {};
    }

    UE_LOG(LogTemp, Warning, TEXT("Processing detections"));
    float x_factor = modelInput.cols / modelShape.width;
    float y_factor = modelInput.rows / modelShape.height;
    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (int i = 0; i < rows; ++i)
    {
            float *classes_scores = data + 4;
        
            cv::Mat scores(1, classes.Num(), CV_32FC1, classes_scores);
            cv::Point class_id;
            double maxClassScore;
        
            minMaxLoc(scores, 0, &maxClassScore, 0, &class_id);

            if (maxClassScore > modelScoreThreshold)
            {
                confidences.push_back(maxClassScore);
                class_ids.push_back(class_id.x);
                                
                float x = data[0];
                float y = data[1];
                float w = data[2];
                float h = data[3];

                int left = int((x - 0.5 * w) * x_factor);
                int top = int((y - 0.5 * h) * y_factor);
                int width = int(w * x_factor);
                int height = int(h * y_factor);

                boxes.push_back(cv::Rect(left, top, width, height));
            }
        
        data += dimensions;
    }

    UE_LOG(LogTemp, Warning, TEXT("Applying Non-Maximum Suppression (NMS)"));
    std::vector<int> nms_result;
    cv::dnn::NMSBoxes(boxes, confidences, modelScoreThreshold, modelNMSThreshold, nms_result);

    
    //UE_LOG(LogTemp, Warning, TEXT("nms size: %d"), nms_result);

    
    TArray<FDetection> detections;
    for (int idx : nms_result) // should be nms.size() but its not it
    {
        FDetection result;
        
        result.class_id = class_ids[idx];
        result.confidence = confidences[idx];
        result.color = FLinearColor(FMath::FRand(), FMath::FRand(), FMath::FRand(), 1.0f); // used to be scalar
        result.className = classes[result.class_id];
        result.box = boxes[idx];
        detections.Add(result);
    }
    UE_LOG(LogTemp, Warning, TEXT("Inference completed with %d detections"), detections.Num());
    return detections;
}

void UInference::loadClassesFromFile()
{
    UE_LOG(LogTemp, Warning, TEXT("Loading classes from file"));
    TArray<FString> FileLines;
    if (FFileHelper::LoadFileToStringArray(FileLines, *classesPath))
    {
        for (const FString& Line : FileLines) // Use const reference to avoid unnecessary copies
        {
            classes.Add(Line);
            UE_LOG(LogTemp, Warning, TEXT("Loaded class: %s"), *Line);
        }
        UE_LOG(LogTemp, Warning, TEXT("Loaded %d classes"), classes.Num());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load classes from %s"), *classesPath);
    }
}

void UInference::loadOnnxNetwork()
{
    UE_LOG(LogTemp, Warning, TEXT("Loading ONNX model from %s"), *modelPath);
    std::string onnxModelPath = TCHAR_TO_UTF8(*modelPath);
    net = cv::dnn::readNetFromONNX(onnxModelPath);
    
    if (net.empty())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load ONNX model!"));
        return;
    }
    
    /*
    if (cudaEnabled) // GPU = true
        {
            net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
            net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
        } else {}
     */

    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    UE_LOG(LogTemp, Warning, TEXT("ONNX model loaded successfully"));
}

cv::Mat UInference::formatToSquare(const cv::Mat &source)
{
    int col = source.cols;
    int row = source.rows;
    int _max = FMath::Max(col, row);
    cv::Mat result = cv::Mat::zeros(_max, _max, CV_8UC3);
    source.copyTo(result(cv::Rect(0, 0, col, row)));
    return result;
}
