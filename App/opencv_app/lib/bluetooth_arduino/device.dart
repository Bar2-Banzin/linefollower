import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import '../main_screens/camera_page.dart';
class BluetoothDeviceListEntry extends StatelessWidget {
  // final Function onTap;
  final BluetoothDevice device;

  const BluetoothDeviceListEntry({Key? key, 
  //  required this.onTap,
     required this.device}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return ListTile(
      onTap:null,
      leading: const Icon(Icons.devices),
      title: Text(device.name ?? "Unknown device"),
      subtitle: Text(device.address.toString()),
      trailing: ElevatedButton(
        child: const Text('Connect'),
        onPressed:()=> {
      
          Navigator.push(
            context,
            MaterialPageRoute(
              builder: (context) {
                return CameraPage(device: device,);
              },
            ),
          )
        },
    
      ),
    );
  }
}