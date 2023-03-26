// ignore_for_file: camel_case_types

import 'dart:async';
import 'dart:ffi';
import 'dart:io';
import 'package:flutter/services.dart';
import 'package:ffi/ffi.dart';

// Load our C lib
final DynamicLibrary nativeLib = Platform.isAndroid
    ? DynamicLibrary.open("libnative_opencv.so")
    : DynamicLibrary.process();

// C Functions signatures
typedef _c_version = Pointer<Utf8> Function();
typedef _c_initDetector = Void Function(
    Pointer<Uint8> markerPngBytes, Int32 inSize, Int32 bits);
typedef _c_destroyDetector = Void Function();
typedef _c_detect = Pointer<Uint8> Function(
    Pointer<Uint8> markerPngBytes, Int32 inSize, Int32 bits);

// Dart functions signatures
typedef _dart_version = Pointer<Utf8> Function();
typedef _dart_initDetector = void Function(
    Pointer<Uint8> markerPngBytes, int inSize, int bits);
typedef _dart_destroyDetector = void Function();
typedef _dart_detect = Pointer<Uint8> Function(
    Pointer<Uint8> markerPngBytes, int inSize, int bits);

// Create dart functions that invoke the C funcion
final _version = nativeLib.lookupFunction<_c_version, _dart_version>('version');
final _initDetector = nativeLib
    .lookupFunction<_c_initDetector, _dart_initDetector>('initDetector');
final _destroyDetector =
    nativeLib.lookupFunction<_c_destroyDetector, _dart_destroyDetector>(
        'destroyDetector');
final _detect = nativeLib.lookupFunction<_c_detect, _dart_detect>('detect');

class NativeOpencv {
  Pointer<Uint8>? _imageBuffer;

  static const MethodChannel _channel = MethodChannel('native_opencv');
  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  String cvVersion() {
    return _version().toDartString();
  }

  void initDetector(Uint8List markerPngBytes, int bits) {
    var totalSize = markerPngBytes.lengthInBytes;
    var imgBuffer = malloc.allocate<Uint8>(totalSize);
    Uint8List bytes = imgBuffer.asTypedList(totalSize);
    bytes.setAll(0, markerPngBytes);

  _initDetector(imgBuffer, totalSize, bits);

    malloc.free(imgBuffer);

  }

  void destroy() {
    _destroyDetector();
    if (_imageBuffer != null) {
      malloc.free(_imageBuffer!);
    }
  }

  Pointer<Uint8> detect(Uint8List markerPngBytes, int bits) {
     var totalSize = markerPngBytes.lengthInBytes;
    var imgBuffer = malloc.allocate<Uint8>(totalSize);
    Uint8List bytes = imgBuffer.asTypedList(totalSize);
    bytes.setAll(0, markerPngBytes);

    Pointer<Uint8> flag = _detect(imgBuffer, totalSize, bits);

    malloc.free(imgBuffer);
    return flag;
  }
}
