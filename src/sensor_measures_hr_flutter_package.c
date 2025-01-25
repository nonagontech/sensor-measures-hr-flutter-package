#include "sensor_measures_hr_flutter_package.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#define FS 100         // 采样率
#define N_ORDER 50     // FIR滤波器阶数：49阶  N-1阶
#define SMOOTH_ORDER 4 // 平滑窗口大小

double Real_Time_FIR_Filter(double *Input_Data)
{
  // 系数
  static double tmp_data[N_ORDER] = {0.00034058022162908124, 0.0002778088207804377, 8.706739959014099e-05, -0.00032351355196518026, -0.001077656729261368, -0.0022975977407538838, -0.004061519490685042, -0.006359587148239215, -0.009057654560323135, -0.01187735707176594, -0.014399300495476152, -0.01609264559454448, -0.01637007891830008, -0.01466264685565504, -0.010504976164381057, -0.003618720715961511, 0.0060188097260108885, 0.01813271517659716, 0.03214400120599771, 0.04720253408824556, 0.06225789226788243, 0.07616092042027583, 0.08778400426745185, 0.09614490994905636, 0.10051800502121139, 0.10051800502121139, 0.09614490994905636, 0.08778400426745188, 0.07616092042027583, 0.06225789226788244, 0.04720253408824557, 0.03214400120599772, 0.018132715176597168, 0.0060188097260108885, -0.0036187207159615125, -0.010504976164381057, -0.014662646855655046, -0.016370078918300085, -0.01609264559454449, -0.01439930049547616, -0.011877357071765954, -0.00905765456032314, -0.006359587148239215, -0.004061519490685043, -0.0022975977407538838, -0.0010776567292613698, -0.00032351355196518026, 8.70673995901411e-05, 0.0002778088207804377, 0.00034058022162908124};

  int Count;
  double Output_Data = 0;

  // Input_Data += N_ORDER - 1;

  for (Count = 0; Count < N_ORDER; Count++)
  {
    Output_Data += (*(tmp_data + Count)) *
                   (*(Input_Data + Count));
  }

  return (double)Output_Data;
}

// 平滑滤波
double smooth_filter(double input)
{
  static double windows[SMOOTH_ORDER] = {0};
  double sum = input;
  for (int i = SMOOTH_ORDER - 1; i > 0; i--)
  {
    sum += windows[i - 1];
    windows[i] = windows[i - 1];
  }
  windows[0] = sum / SMOOTH_ORDER;

  return windows[0];
}
// 包络检波
double envelope_filter(double x, double rct)
{
  static double old_y = 0.0;
  if (rct == 0.0)
  {
    old_y = 0.0;
  }
  else
  {
    x = fabs(x);
    if (x > old_y)
    {
      old_y = x;
    }
    else
    {
      old_y *= rct / (rct + 1);
    }
  }

  return old_y;
}
// 心率平滑
int hr_filter(int input)
{
  static int pre_heart_rate = 0;
  static int output = 0;
  static int zero_cnt = 0;
  static int change_cnt = 0;
  const static int zero_threshold = 3;
  const static int change_threshold = 3;

  if (pre_heart_rate == 0)
  {
    output = input;
    zero_cnt = 0;
  }
  else
  {
    if (input == 0)
    {
      zero_cnt++;
      if (zero_cnt > zero_threshold)
      {
        output = 0;
      }
    }
    else
    {
      if (pre_heart_rate != input)
      {
        if (pre_heart_rate < input)
          output = pre_heart_rate + (input - pre_heart_rate) / 5 + 1;
        else if (pre_heart_rate > input)
          output = pre_heart_rate - (pre_heart_rate - input) / 5 - 1;
        change_cnt++;
        if (change_cnt > change_threshold)
        {
          output = pre_heart_rate;
          change_cnt = 0;
        }
      }
      else
      {
        change_cnt = 0;
      }
      zero_cnt = 0;
    }
  }
  pre_heart_rate = output;

  return output;
}

// 心率求解
FFI_PLUGIN_EXPORT int hr_algorithim_lib(double *data, int size, int fs)
{
  int hr = 0;
  int peak_index[100] = {0}; // 存放峰值点下标
  int peak_cnt = 0;
  double max_val = data[0], min_val = data[0];
  int i, j, k;
  int is_valid_peak = 1;

  // 求最大最小值
  for (i = 0; i < size; i++)
  {
    if (data[i] > max_val)
      max_val = data[i];
    if (data[i] < min_val)
      min_val = data[i];
  }
  if ((max_val - min_val) == 0)
  {
    return 0;
  }

  // 查找峰值点，峰值点需大于最大值、最小值的一半，峰值间隔大于10（采样点）
  double threshold = (max_val + min_val) / 2.0;
  double dc_threshold = (max_val - min_val) / 2.0;
  // printf("%lf\t%lf\n", dc_threshold, threshold);

  for (i = 10; i < size - 10; i++)
  { // 忽略开始和结束部分的数据，确保有足够的间隔空间
    if ((data[i] > threshold) &&
        (data[i - 1] < data[i]) && (data[i + 1] < data[i]))
    { // 检查是否为局部最大值
      is_valid_peak = 1;
      for (j = i - 10; j < i && is_valid_peak; j++)
      { // 检查前10个点
        if (data[j] > data[i])
          is_valid_peak = 0;
      }
      for (j = i + 1; j < i + 11 && is_valid_peak; j++)
      { // 检查后10个点
        if (data[j] > data[i])
          is_valid_peak = 0;
      }
      if (is_valid_peak)
      { // 找到峰值点
        for (k = i; k < size && ((k - i) < fs / 4); k++)
        { // 查找前一个谷值点
          if (data[k] < data[k + 1])
            break;
        }
        // printf("%d\t%d\t%lf\n", i, k-i, data[i]-data[k]);

        if ((data[i] - data[k]) > dc_threshold && peak_cnt < 100)
        { // 上升高度阈值
          peak_index[peak_cnt++] = i;
        }
      }
    }
  }
  // printf("%d\n", peak_cnt);
  // 通过峰值点下标判断心率
  if (peak_cnt < 3)
    return hr_filter(0); // 至少需要两个峰值点来计算心率

  double avg_peak_interval = 0;
  int min = size, max = 0;
  for (i = 1; i < peak_cnt; i++)
  {
    avg_peak_interval += (peak_index[i] - peak_index[i - 1]);
    if (peak_index[i] - peak_index[i - 1] > max)
      max = (peak_index[i] - peak_index[i - 1]);
    if (peak_index[i] - peak_index[i - 1] < min)
      min = (peak_index[i] - peak_index[i - 1]);
  }
  avg_peak_interval /= (peak_cnt - 1);

  // 计算心率（每分钟的心跳次数）
  if (max / min >= 2)
    hr = 0; // 间隔之间相差太大
  else
    hr = (int)(60.0 * fs / avg_peak_interval);

  // 心率大于300或小于30，判断为无效心率，返回0
  if (hr > 300 || hr < 30)
  {
    hr = 0;
  }

  return hr_filter(hr);
  // return hr;
}

// 滤波算法
FFI_PLUGIN_EXPORT double fir_filter_lib(double input)
{
  static double next_data = 0;
  static double last_filtered_data[N_ORDER]; // 输入缓存
  double pre_data = 0, absolute_data = 0, smooth_data = 0, out_data, envelope_data = 0;

  // 差分取绝对值
  pre_data = next_data;
  next_data = input;
  absolute_data = fabs(next_data - pre_data);

  // 包络检波
  envelope_data = envelope_filter(absolute_data, FS / 5);

  // 插入数据至队列
  for (int i = N_ORDER - 1; i > 0; i--)
  {
    last_filtered_data[i] = last_filtered_data[i - 1];
  }
  last_filtered_data[0] = envelope_data;

  // 滤波
  out_data = Real_Time_FIR_Filter(last_filtered_data);

  // 平滑
  smooth_data = smooth_filter(out_data);

  // 更新滤波数据
  return smooth_data;
}
