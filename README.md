## 使用方法
### 安装依赖
  ``` 
  dependencies:
    sensor_measures_hr_flutter_package:
      git:
        url: https://github.com/nonagontech/sensor-measures-hr-flutter-package.git
    ffi: any
  ```
### 导入
  ```dart
  import 'dart:ffi';
  import "package:ffi/ffi.dart";  
  import 'package:sensor_measures_hr_flutter_package/sensor_measures_hr_flutter_package.dart' as sensor_measures_hr_flutter_package;
  ```
### 函数说明
#### firFilterLib
**描述**: 对数据进行FIR滤波器处理

```dart
double hrAlgorithimLib(double input)
```
##### 参数
| 参数名  | 类型     | 描述                 | 默认值 |
| ------- | -------- | -------------------- | ------ |
| `input` | `double` | 输入信号的一个样本值 | 无     |
 
##### 返回值
- 返回一个double类型的返回经过“滤波”后值


#### hrAlgorithimLib
**描述**: 计算心率
 
```dart
int hrAlgorithimLib(Pointer<Double> data, int size, int fs)
```
##### 参数

| 参数名 | 类型              | 描述                             | 默认值 |
| ------ | ----------------- | -------------------------------- | ------ |
| `data` | `Pointer<Double>` | 指向 指向数据数组的指针          | 无     |
| `size` | `int`             | 数据数组的大小（即数据点的数量） | 无     |
| `fs`   | `int`             | 采样频率（每秒采样点数）         | 无     |

##### 返回值
- 返回一个int类型的心率值

 
### 举例
```dart
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

```