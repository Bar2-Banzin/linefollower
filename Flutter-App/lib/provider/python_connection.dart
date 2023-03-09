import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';
import 'dart:io';
import 'package:path/path.dart';
import 'package:async/async.dart';

class SendImage with ChangeNotifier {
  Future<void> sendImage(File imageFile) async {
    // open a bytestream
      print("====================================================================================");

      var stream = new http.ByteStream(DelegatingStream.typed(imageFile.openRead()));
      // get file length
      var length = await imageFile.length();
        var url ="http://127.0.0.1:5000/upload";
  http.MultipartRequest request = http.MultipartRequest('POST', Uri.parse(url));
  request.files.add(
    await http.MultipartFile(
      'file', stream, length,
          filename: basename(imageFile.path)
    )
  );
  http.StreamedResponse r = await request.send();
  print(r.statusCode);
  print(await r.stream.transform(utf8.decoder).join());
  }
}
