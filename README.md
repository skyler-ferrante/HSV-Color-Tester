# HSV-Color-Tester
Simple OpenCV program that can be used to find upper and bottom bounds for the inRange function, on a image or from the camera.

Make sure you have the OpenCV library. Then you can build with:

    $ cmake . 
    $ make
Then run with:

    $ ./HSV_COLOR_TESTER
  
You can also change whether you want to use the camera/target.jpg by commenting or not commenting 
    
    #define USE_IMAGE
