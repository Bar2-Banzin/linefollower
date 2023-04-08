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
  Int32 width,
  Int32 height,
  Pointer<Uint8> bytes,
  Bool isYUV,
);

// Dart functions signatures
typedef _dart_version = Pointer<Utf8> Function();
typedef _dart_initDetector = void Function(
    Pointer<Uint8> markerPngBytes, int inSize, int bits);
typedef _dart_destroyDetector = void Function();
typedef _dart_detect = Pointer<Uint8> Function(
  int width,
  int height,
  Pointer<Uint8> bytes,
  bool isYUV,
);

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

  Pointer<Uint8> detect(int width, int height, Uint8List yBuffer,
      Uint8List? uBuffer, Uint8List? vBuffers) {
    var ySize = yBuffer.lengthInBytes;
    var uSize = uBuffer?.lengthInBytes ?? 0;
    var vSize = vBuffers?.lengthInBytes ?? 0;
    var totalSize = ySize + uSize + vSize;

    _imageBuffer ??= malloc.allocate<Uint8>(totalSize);
    // We always have at least 1 plane, on Android it si the yPlane on iOS its the rgba plane
    Uint8List bytes = _imageBuffer!.asTypedList(totalSize);
    bytes.setAll(0, yBuffer);

    if (Platform.isAndroid) {
      // Swap u&v buffer for opencv
      bytes.setAll(ySize, vBuffers!);
      bytes.setAll(ySize + vSize, uBuffer!);
    }
    Pointer<Uint8> flag = _detect(
        width, height, _imageBuffer!, Platform.isAndroid ? true : false);

    return flag;
  }
}
