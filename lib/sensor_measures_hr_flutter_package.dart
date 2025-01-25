import 'dart:ffi';
import 'dart:io';

import 'sensor_measures_hr_flutter_package_bindings_generated.dart';

int hrAlgorithimLib(Pointer<Double> data, int size, int fs) =>
    _bindings.hr_algorithim_lib(data, size, fs);

double firFilterLib(double input) => _bindings.fir_filter_lib(input);

const String _libName = 'sensor_measures_hr_flutter_package';

/// The dynamic library in which the symbols for [SensorMeasuresHrFlutterPackageBindings] can be found.
final DynamicLibrary _dylib = () {
  if (Platform.isMacOS || Platform.isIOS) {
    return DynamicLibrary.open('$_libName.framework/$_libName');
  }
  if (Platform.isAndroid || Platform.isLinux) {
    return DynamicLibrary.open('lib$_libName.so');
  }
  if (Platform.isWindows) {
    return DynamicLibrary.open('$_libName.dll');
  }
  throw UnsupportedError('Unknown platform: ${Platform.operatingSystem}');
}();

/// The bindings to the native functions in [_dylib].
final SensorMeasuresHrFlutterPackageBindings _bindings =
    SensorMeasuresHrFlutterPackageBindings(_dylib);
