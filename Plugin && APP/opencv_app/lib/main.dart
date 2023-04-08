import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'main_screens/start_page.dart';
import './bluetooth_arduino/connection.dart';
import './main_screens/camera_page.dart';
void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        primarySwatch: Colors.blue,
        visualDensity: VisualDensity.adaptivePlatformDensity,
      ),
      home: FutureBuilder(
        future: FlutterBluetoothSerial.instance.requestEnable(),
        builder: (context, future) {
          if (future.connectionState == ConnectionState.waiting) {
            return const Scaffold(
              body: SizedBox(
                height: double.infinity,
                child: Center(
                  child: Icon(
                    Icons.bluetooth_disabled,
                    size: 200.0,
                    color: Colors.blue,
                  ),
                ),
              ),
            );
          } else if (future.connectionState == ConnectionState.done) {
            // return MyHomePage(title: 'Flutter Demo Home Page');
            return const Home();
          } else {
            return const Home();
          }
        },
        // child: MyHomePage(title: 'Flutter Demo Home Page'),
      ),
      routes: {
        SelectBondedDevicePage.routeName: (context) => const SelectBondedDevicePage(),
      },
    );
  }
}

class Home extends StatelessWidget {
  const Home({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return SafeArea(
        child: Scaffold(
            appBar: AppBar(
              title: const Text('Lightning McQueen'),
            ),
            body:
            const StartPage()
            ));
  }
}

//=============================================================================================================================//
// import 'dart:developer';

// import 'package:flutter/material.dart';
// import 'package:flutter/services.dart';
// import 'package:native_opencv/native_opencv.dart';
// import 'package:opencv_app/detection/detection_page.dart';

// void main() async {
//   WidgetsFlutterBinding.ensureInitialized();
//   await SystemChrome.setPreferredOrientations([DeviceOrientation.portraitUp, DeviceOrientation.portraitDown]);

//   runApp(const OpenCVApp());
// }

// class OpenCVApp extends StatelessWidget {
//   const OpenCVApp({Key? key}) : super(key: key);

//   @override
//   Widget build(BuildContext context) {
//     return const MaterialApp(
//       title: 'Flutter Demo',
//       home: HomePage(),
//     );
//   }
// }

// class HomePage extends StatelessWidget {
//   const HomePage({Key? key}) : super(key: key);

//   @override
//   Widget build(BuildContext context) {
//     return Container(
//       color: Colors.white,
//       child: Center(
//         child: ElevatedButton(
//           child: const Text('Camera'),
//           onPressed: () {
//             Navigator.of(context).push(MaterialPageRoute(builder: (ctx) {
//               return const DetectionPage();
//             }));
//           },
//         ),
//       ),
//     );
//   }
// }