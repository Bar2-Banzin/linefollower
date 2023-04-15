import 'dart:async';
import 'dart:ffi';
import 'dart:typed_data';
import 'package:camera/camera.dart';
import 'package:flutter/material.dart';
import 'package:native_opencv/native_opencv.dart';

class ImageProcessing {
  NativeOpencv? _nativeOpencv;
  Future<int> detect(CameraImage image,int rotation) async {
    _nativeOpencv = NativeOpencv();
    var planes = image.planes;
    var yBuffer = planes[0].bytes;

    Uint8List? uBuffer;
    Uint8List? vBuffer;

    uBuffer = planes[1].bytes;
    vBuffer = planes[2].bytes;
    var start = DateTime.now();
    print(
        "=================Start Time==============${start.hour}hr ${start.minute} min ${start.second} sec ${start.millisecond} msec ");
    var res = _nativeOpencv!
        .detect(image.width, image.height, yBuffer, uBuffer, vBuffer,rotation);
    var end = DateTime.now();
    print(
        "=================End Time==============${end.hour}hr ${end.minute} min ${end.second} sec ${end.millisecond} msec ");

    print("=================image processing result${res.value}");
    return res.value;
  }

  Future<int> first_detect(XFile image) async {
    print('=================First time================');
    Uint8List _output = await image.readAsBytes();
    _nativeOpencv = NativeOpencv();
    var res = _nativeOpencv!.initDetector(_output, 36);
     return Future.value(res.value);

  }
}
