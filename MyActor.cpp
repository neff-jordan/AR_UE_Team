

#include "MyActor.h"

// Sets default values
AMyActor::AMyActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
PrimaryActorTick.bCanEverTick = true;

}

void AMyActor::TestOpenCV(){
UE_LOG(LogTemp, Warning, TEXT("Testing OpenCV..."));
int testDim[3] = {2, 3, 4};
cv::Mat testMat(3, testDim, CV_32FC1);
UE_LOG(
       LogTemp, Warning,
    TEXT("dimension = %d, %d, %d"),
    testMat.size[0], testMat.size[1], testMat.size[2]);
UE_LOG(LogTemp, Warning, TEXT("Testing Done!"));
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
Super::BeginPlay();
TestOpenCV();

}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
Super::Tick(DeltaTime);

}
