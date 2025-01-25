import 'dart:ffi';
import "package:ffi/ffi.dart";

import 'package:flutter/material.dart';

import 'package:sensor_measures_hr_flutter_package/sensor_measures_hr_flutter_package.dart'
    as sensor_measures_hr_flutter_package;

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  late int hrAlgorithim;
  late double firFilter;
  List<double> testList = [1, 2, 3, 4, 5];

  @override
  void initState() {
    super.initState();
    Pointer<Double> pointer = calloc<Double>(testList.length);
    hrAlgorithim = sensor_measures_hr_flutter_package.hrAlgorithimLib(
        pointer, testList.length, 10);
    firFilter = sensor_measures_hr_flutter_package.firFilterLib(100);
  }

  @override
  Widget build(BuildContext context) {
    const textStyle = TextStyle(fontSize: 25);
    const spacerSmall = SizedBox(height: 10);
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Native Packages'),
        ),
        body: SingleChildScrollView(
          child: Container(
            padding: const EdgeInsets.all(10),
            child: Column(
              children: [
                Text(
                  'hrAlgorithim = $hrAlgorithim',
                  style: textStyle,
                  textAlign: TextAlign.center,
                ),
                spacerSmall,
                Text(
                  'firFilter = $firFilter',
                  style: textStyle,
                  textAlign: TextAlign.center,
                ),
                ElevatedButton(
                    onPressed: () {
                      Pointer<Double> pointer = calloc<Double>(testList.length);
                      for (int i = 0; i < testList.length; i++) {
                        pointer[i] = testList[i];
                      }

                      hrAlgorithim = sensor_measures_hr_flutter_package
                          .hrAlgorithimLib(pointer, testList.length, 10);
                      setState(() {});
                    },
                    child: const Text("心率"))
              ],
            ),
          ),
        ),
      ),
    );
  }
}
