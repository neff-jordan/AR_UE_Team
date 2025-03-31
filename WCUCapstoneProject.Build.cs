using UnrealBuildTool;
using System.IO;
using System.Diagnostics;

public class WCUCapstoneProject : ModuleRules
{
    private string OpenCVIncludePath1 = "/Users/jordanneff/opencv/modules/core/include";
    private string OpenCVIncludePath2 = "/Users/jordanneff/opencv/modules/gapi/include";
    private string OpenCVLibPath = "/Users/jordanneff/opencv_build/lib";

    public bool LoadOpenCV(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            isLibrarySupported = true;

            PublicIncludePaths.Add(OpenCVIncludePath1);
            PublicIncludePaths.Add(OpenCVIncludePath2);
            PublicIncludePaths.Add("/Users/jordanneff/opencv_build");
            PublicIncludePaths.Add("/Users/jordanneff/opencv_build/opencv2");
            PublicIncludePaths.Add("/Users/jordanneff/opencv_build/lib");
            PublicIncludePaths.Add("/Users/jordanneff/opencv/modules/dnn/include"); // tried /opencv2/dnn
            PublicIncludePaths.Add("/Users/jordanneff/opencv/modules/imgcodecs/include"); // /opencv2/imgcodecs

            // /Users/jordanneff/opencv/modules
            // /Users/jordanneff/opencv/modules/imgcodecs/include/opencv2/imgcodecs
            
            
            string[] OpenCVLibs = new string[]
            {
                "libopencv_core.dylib",
                "libopencv_imgproc.dylib",
                "libopencv_highgui.dylib",
                "libopencv_imgcodecs.dylib",
                "libopencv_videoio.dylib",
                "libopencv_dnn.dylib",
                "libopencv_dnn.4.7.0.dylib",
                "libopencv_dnn.407.dylib"
            };

            foreach (string lib in OpenCVLibs)
            {
                PublicAdditionalLibraries.Add(Path.Combine(OpenCVLibPath, lib));
            }

            foreach (string lib in OpenCVLibs)
            {
                RuntimeDependencies.Add(Path.Combine(OpenCVLibPath, lib));
            }
        }
        else
        {
            System.Console.WriteLine("OpenCV is not configured for this platform.");
        }

        PublicDefinitions.Add(string.Format("WITH_OPENCV_BINDING={0}", isLibrarySupported ? 1 : 0));
        return isLibrarySupported;
    }

    public WCUCapstoneProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        IWYUSupport = IWYUSupport.Full;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core", "CoreUObject",
            "Engine", "InputCore",
            "RHI", "RenderCore",
            "Media", "MediaAssets",
            "EnhancedInput"
        });

        LoadOpenCV(Target);
    }
}
