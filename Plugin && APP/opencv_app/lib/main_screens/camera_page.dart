import 'package:flutter/material.dart';
import 'package:camera/camera.dart';
import 'package:responsive_sizer/responsive_sizer.dart';
import 'dart:developer';
import 'dart:io';
import 'dart:async';
import 'dart:typed_data';
import '../detection_line_curve/image_processing.dart';
import 'package:native_opencv/native_opencv.dart';

class CameraPage extends StatefulWidget {
  CameraPage({Key? key}) : super(key: key);

  @override
  State<CameraPage> createState() => _CameraPageState();
}

class _CameraPageState extends State<CameraPage> with WidgetsBindingObserver {
  CameraController? _camController;
  int _camFrameRotation = 0;
  double _camFrameToScreenScale = 0;
  int _lastRun = 0;
  bool _detectionInProgress = false;
  ImageProcessing? _imageProcessing;
  NativeOpencv? _nativeOpencv;
  bool first_time = true;
  var zooming = 0.0;
  @override
  void initState() {
    super.initState();
    WidgetsBinding.instance.addObserver(this);
    _imageProcessing = ImageProcessing();
    initCamera();
  }

  @override
  void didChangeAppLifecycleState(AppLifecycleState state) {
    final CameraController? cameraController = _camController;

    // App state changed before we got the chance to initialize.
    if (cameraController == null || !cameraController.value.isInitialized) {
      return;
    }

    if (state == AppLifecycleState.inactive) {
      cameraController.dispose();
    } else if (state == AppLifecycleState.resumed) {
      initCamera();
    }
  }

  @override
  void dispose() {
    WidgetsBinding.instance.removeObserver(this);
    _camController?.dispose();
    super.dispose();
  }

  Future<void> initCamera() async {
    final cameras = await availableCameras();
    var idx =
        cameras.indexWhere((c) => c.lensDirection == CameraLensDirection.back);
    if (idx < 0) {
      log("No Back camera found - weird");
      return;
    }

    var desc = cameras[idx];
    _camFrameRotation = Platform.isAndroid ? desc.sensorOrientation : 0;
    _camController = CameraController(
      desc,
      ResolutionPreset.high, // 720p
      enableAudio: false,
      imageFormatGroup: Platform.isAndroid
          ? ImageFormatGroup.yuv420
          : ImageFormatGroup.bgra8888,
    );

    try {
      await _camController!.initialize();
      // await _camController!.startImageStream((image) => _processCameraImage(image));
    } catch (e) {
      log("Error initializing camera, error: ${e.toString()}");
    }

    if (mounted) {
      setState(() {});
    }
  }

  void _processCameraImage() async {
    try {
      var image = await _camController!.takePicture();
      if (first_time) {
        await _imageProcessing!.first_detect(image);
        first_time = false;
      } else
        await _imageProcessing!.detect(image);
      Navigator.push(
        context,
        MaterialPageRoute(
          builder: (context) {
            return DisplayPictureScreen(
              imagePath: image.path,
            );
          },
        ),
      );
      setState(() {});
    } catch (e) {
      // If an error occurs, log the error to the console.
      print("Error ${e}");
    }
  }

  void _Zoom_in() async{
    setState(() {
      zooming++;
    });
        await  _camController!.setZoomLevel(zooming);
  }

  void _Zoom_out()async {
    setState(() {
      zooming--;
    });
     await  _camController!.setZoomLevel(zooming);
  }

  @override
  Widget build(BuildContext context) {
    if (_camController == null) {
      return const Center(
        child: Text('Loading...'),
      );
    }

    return Column(
      children: [
        Container(
          height: 600,
          child: CameraPreview(_camController!),
        ),
        
        Row(
          children: [
            FloatingActionButton(
              heroTag: "btn1",
              // Provide an onPressed callback.
              onPressed: () => _processCameraImage(),
              child: const Icon(Icons.camera_alt),
            ),
            FloatingActionButton(
              heroTag: "btn2",
              // Provide an onPressed callback.
              onPressed: () => _Zoom_in(),
              child: const Icon(Icons.add),
            ),
            FloatingActionButton(
              heroTag: "btn3",
              // Provide an onPressed callback.
              onPressed: () => _Zoom_out(),
              child: const Icon(Icons.remove),
            ),
          ],
        )
      ],
    );
  }
}

// A widget that displays the picture taken by the user.
class DisplayPictureScreen extends StatelessWidget {
  final String imagePath;

  const DisplayPictureScreen({required this.imagePath});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Display the Picture')),
      // The image is stored as a file on the device. Use the `Image.file`
      // constructor with the given path to display the image.
      body: Image.file(File(imagePath)),
    );
  }
}
