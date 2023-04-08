import 'package:flutter/material.dart';
import 'package:camera/camera.dart';
import 'dart:developer';
import 'dart:io';
import 'dart:async';
import 'dart:typed_data';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import '../detection_line_curve/image_processing.dart';
import 'package:native_opencv/native_opencv.dart';
import 'package:gallery_saver/gallery_saver.dart';
import 'dart:convert';

class CameraPage extends StatefulWidget {
  final BluetoothDevice device;
  const CameraPage({Key? key, required this.device}) : super(key: key);

  @override
  State<CameraPage> createState() => _CameraPageState();
}

class _Message {
  int whom;
  String text;

  _Message(this.whom, this.text);
}

class _CameraPageState extends State<CameraPage> with WidgetsBindingObserver {
  CameraController? _camController;
  int _camFrameRotation = 0;
  ImageProcessing? _imageProcessing;
  late BluetoothConnection connection;

  List<_Message> messages = [];
  String _messageBuffer = '';

  final TextEditingController textEditingController = TextEditingController();
  final ScrollController listScrollController = ScrollController();

  bool isConnecting = true;
  bool get isConnected => connection.isConnected;

  bool isDisconnecting = false;
  Timer? timer;
  int _lastRun = 0;
  bool _detectionInProgress = false;
  NativeOpencv? _nativeOpencv;
  bool first_time = true;
  var zooming = 0.0;
  @override
  void initState() {
    super.initState();
    WidgetsBinding.instance.addObserver(this);
    _imageProcessing = ImageProcessing();
    initCamera();
    BluetoothConnection.toAddress(widget.device.address).then((_connection) {
      print('Connected to the device');
      connection = _connection;
      setState(() {
        isConnecting = false;
        isDisconnecting = false;
      });

      connection.input!.listen(_onDataReceived).onDone(() {
        if (isDisconnecting) {
          print('Disconnecting locally!');
        } else {
          print('Disconnected remotely!');
        }
        if (mounted) {
          setState(() {});
        }
      });
    }).catchError((error) {
      print('Cannot connect, exception occured');
      print(error);
    });
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
    if (isConnected) {
      isDisconnecting = true;
      connection.dispose();
      // connection = null;
    }
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
    } catch (e) {
      log("Error initializing camera, error: ${e.toString()}");
    }

    if (mounted) {
      setState(() {});
    }
  }

  void take_photo_first() async {
    var image = await _camController!.takePicture();
    await GallerySaver.saveImage(image.path);
    await _imageProcessing!.first_detect(image);
    setState(() {
      first_time = false;
    });
    print("First Time$first_time");
  }

  void take_photo_Multi() async {
    await _camController!
        .startImageStream((image) => _processCameraImage(image));
    // timer = Timer.periodic(
    //     const Duration(milliseconds: 300), (Timer t) => _processCameraImage());
  }

  void _processCameraImage(CameraImage image) async {
    if (_detectionInProgress ||
        DateTime.now().millisecondsSinceEpoch - _lastRun < 60) {
      return;
    }
    try {
      print("===========================in Streaming=============");
      // var image = await _camController!.takePicture();
      // await GallerySaver.saveImage(image);
      _detectionInProgress = true;
      var res = await _imageProcessing!.detect(image);
      _detectionInProgress = false;
      _lastRun = DateTime.now().millisecondsSinceEpoch;
      print("Result of process${res.toString()}");
      _sendMessage(res.toString());
    } catch (e) {
      // If an error occurs, log the error to the console.
      print("Error $e");
    }
  }

  void _Zoom_in() async {
    setState(() {
      zooming++;
    });
    await _camController!.setZoomLevel(zooming);
  }

  void _Zoom_out() async {
    setState(() {
      zooming--;
    });
    await _camController!.setZoomLevel(zooming);
  }

  void _sendMessage(String text) async {
    text = text.trim();
    textEditingController.clear();

    if (text.isNotEmpty) {
      try {
        connection.output.add(Uint8List.fromList(utf8.encode(text)));
        await connection.output.allSent;

        // setState(() {
        //   messages.add(_Message(clientID, text));
        // });
        DateTime dt;
        // Future.delayed(const Duration(milliseconds: 333)).then((_) {
        dt = DateTime.now();
        print(
            "=================Time==============${dt.hour}hr ${dt.minute} min ${dt.second} sec ${dt.millisecond} msec ");
        //   listScrollController.animateTo(
        //       listScrollController.position.maxScrollExtent,
        //       duration: const Duration(milliseconds: 333),
        //       curve: Curves.easeOut);
        // });
      } catch (e) {
        // Ignore error, but notify state
        setState(() {});
      }
    }
  }

  void _onDataReceived(Uint8List data) {
    // Allocate buffer for parsed data
    int backspacesCounter = 0;
    for (var byte in data) {
      if (byte == 8 || byte == 127) {
        backspacesCounter++;
      }
    }
    Uint8List buffer = Uint8List(data.length - backspacesCounter);
    int bufferIndex = buffer.length;

    // Apply backspace control character
    backspacesCounter = 0;
    for (int i = data.length - 1; i >= 0; i--) {
      if (data[i] == 8 || data[i] == 127) {
        backspacesCounter++;
      } else {
        if (backspacesCounter > 0) {
          backspacesCounter--;
        } else {
          buffer[--bufferIndex] = data[i];
        }
      }
    }

    // Create message if there is new line character
    String dataString = String.fromCharCodes(buffer);
    int index = buffer.indexOf(13);
    if (~index != 0) {
      setState(() {
        messages.add(
          _Message(
            1,
            backspacesCounter > 0
                ? _messageBuffer.substring(
                    0, _messageBuffer.length - backspacesCounter)
                : _messageBuffer + dataString.substring(0, index),
          ),
        );
        _messageBuffer = dataString.substring(index);
      });
    } else {
      _messageBuffer = (backspacesCounter > 0
          ? _messageBuffer.substring(
              0, _messageBuffer.length - backspacesCounter)
          : _messageBuffer + dataString);
    }
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
        SizedBox(
          height: 600,
          child: CameraPreview(_camController!),
        ),
        Row(
          children: [
            FloatingActionButton(
              heroTag: "btn1",
              // Provide an onPressed callback.
              onPressed: () =>
                  (first_time) ? take_photo_first() : take_photo_Multi(),
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
// class DisplayPictureScreen extends StatelessWidget {
//   final String imagePath;
//   const DisplayPictureScreen({Key? key, required this.imagePath}) : super(key: key);
//   @override
//   Widget build(BuildContext context) {
//     return Scaffold(
//       appBar: AppBar(title: const Text('Display the Picture')),
//       // The image is stored as a file on the device. Use the `Image.file`
//       // constructor with the given path to display the image.
//       body: Image.file(File(imagePath)),
//     );
//   }
// }
