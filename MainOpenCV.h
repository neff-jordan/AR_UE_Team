// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inference.h"

#include "UObject/NoExportTypes.h"

// Undefine Apple's 'check' macro to prevent conflicts
#ifdef check
#undef check
#endif

#include <opencv2/core.hpp> // might be <> instead of "" idk
#include <opencv2/opencv_modules.hpp>

#include "MainOpenCV.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class WCUCAPSTONEPROJECT_API UMainOpenCV : public UObject
{
	GENERATED_BODY()
    
    public:
        UFUNCTION(BlueprintCallable, Category = "Inference")
        static int Start();
	
};
