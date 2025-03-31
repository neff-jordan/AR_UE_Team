// Fill out your copyright notice in the Description page of Project Settings.


/*
 Example class to test out OpenCV connection
 */


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Undefine Apple's 'check' macro to prevent conflicts
#ifdef check
#undef check
#endif

//#include "PreOpenCVHeaders.h"
//#include "OpenCVHelper.h" // Ensure OpenCVHelper is included first
//#include <opencv2/imgproc.hpp>
//#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp> // might be <> instead of "" idk
#include <opencv2/opencv_modules.hpp>



//#include "PostOpenCVHeaders.h"
//#include "Log.h"

#include "MyActor.generated.h"

UCLASS()
class WCUCAPSTONEPROJECT_API AMyActor : public AActor
{
    GENERATED_BODY()

    public:
        // Sets default values for this actor's properties
        AMyActor();
        void TestOpenCV();

    protected:
        // Called when the game starts or when spawned
        virtual void BeginPlay() override;

    public:
        // Called every frame
        virtual void Tick(float DeltaTime) override;

};
