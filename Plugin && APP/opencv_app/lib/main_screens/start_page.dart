import 'package:flutter/material.dart';
import '../bluetooth_arduino/connection.dart';

class StartPage extends StatelessWidget {
  const StartPage({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        body: Container(
            constraints: const BoxConstraints.expand(),
            decoration: const BoxDecoration(
              image: DecorationImage(
                  image: AssetImage("assets/car.jpg"), fit: BoxFit.cover),
            ),
            child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceAround,
              children: [
                Row(
                  mainAxisAlignment: MainAxisAlignment.spaceAround,
                  children: [
                    ElevatedButton(onPressed: () => {}, child: const Text('About')),
                    ElevatedButton(
                        onPressed: () => {
                              Navigator.of(context)
                                  .pushNamed(SelectBondedDevicePage.routeName)
                            },
                        child: const Text('Start')),
                  ],
                )
              ],
            )));
  }
}
