import 'dart:async';
import 'dart:ffi';
import 'dart:typed_data';
import 'package:camera/camera.dart';
import 'package:flutter/material.dart';
import 'package:native_opencv/native_opencv.dart';

class ImageProcessing {
  NativeOpencv? _nativeOpencv;
  Future<int> detect(XFile image) async {
    Uint8List _output = await image.readAsBytes();
    var decodedImage = await decodeImageFromList(_output);
    int imgWidth = decodedImage.width;
    int imgHeight = decodedImage.height;
    _nativeOpencv = NativeOpencv();
    final res = _nativeOpencv!.detect(_output, 36);
    print("=================image processing result${res.value}");
    return Future.value(1);
  }

  Future<int> first_detect(XFile image) async {
    print('=================First time================');
    Uint8List _output = await image.readAsBytes();
    var decodedImage = await decodeImageFromList(_output);
    int imgWidth = decodedImage.width;
    int imgHeight = decodedImage.height;
    _nativeOpencv = NativeOpencv();
    _nativeOpencv!.initDetector(_output, 36);
    return Future.value(1);
  }  
}
