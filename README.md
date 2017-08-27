# Neon Rivers

An interactive XBox One Kinect LED sculpture that uses the Art-Net and DMX protocols with Madrix hardware to create a flowing, pulsing river of light.

## XBox Kinect One Setup

### OpenFrameworks
1. [Download openFrameworks for Visual Studio 2015](http://openframeworks.cc/download/)
2. Install Visual Studio 2015 from OF setup guide (previous link)
3. Follow the Visual Studio setup guide to add openFrameworks plugin
4. Move openframeworks directory  to your C drive, e.g.: ``C:\of_v0.9.8_vs_release`` (prevents a bug)

### Kinect
1. Install Kinect SDK https://www.microsoft.com/en-gb/download/confirmation.aspx?id=44561
2. On GitHub, download zipped folder ofxKinectForWindows2 **making sure to use tag 0.9.0** [click here](https://github.com/elliotwoods/ofxKinectForWindows2/tree/0.9.0)
3. Once this addon is downloaded, add it to the following directory: ``C:\of_v0.9.8_vs_release\addons``
4. Delete the tag “0.9.0” from the Kinect addon directory name so that you now have: ``C:\of_v0.9.8_vs_release\addons\ofxKinectForWindows2``

### Test
1. Test SDK: ``Windows Explorer > Kinect SDK > Kinect Studio``
2. Test openFrameworks Kinect Windows 2 example file (File > open project / solution) ``C:\of_v0.9.8_vs_release\addons\ofxKinectForWindows2\example.sln``
3. Change the platform (at the top in Visual Studio) from win32 to x64
4. Click the arrow to build the project.


## Creating a New openFrameworks Project
1. Make your project with Project Generator, but do not select ofxKinectForWindows2 as an addon at this stage
2. Right click on the Solution and choose ``Add > Existing Project > ofxKinectForWindows2Lib.vcxproj``
3. In Property Manager (open it from View -> Other Windows -> Property Manager), right click on your project to select
Add Existing Property Sheet... and select the ofxKinectForWindows2.props file.
4. Go back to Solution Explorer, right click on your project (e.g. 'mySketch') and select 'Add Reference...', and add a reference to ofxKinectForWindows2Lib.

Include library to ofApp.h file:
#include "ofxKinectForWindows2.h"
Create kinect object identifier in ofApp.h:
ofxKFW2::Device kinect;
