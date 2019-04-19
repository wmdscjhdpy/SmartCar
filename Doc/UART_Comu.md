# 智能小车视觉-电控协议 
*V1.0 2019年4月19日09:27:55 By WMD*
## 数据帧结构
```C
typedef struct 
{
    uint8_t Head;
    uint8_t State;
    uint8_t adden1;
    uint8_t adden2;
    float XData;
    float YData;
    uint8_t End;
}Rec_flame_t;
```
## 通用帧协定
- Head=0x4f
- End为前面所有字节和取低八位

## STM32->OpenMV 通信数据帧
1. OpenMV 视觉工作指令使能
    - State=0x01
    - adden1=:
        - 0x01 物体追踪指令
        - 0x02 面部表情识别
        - 0x03 车辆有无运动指令

## OpenMV->STM32 通信数据帧
1. OpenMV 视觉工作完成标志
    - State=0x02
    - adden1=:
        - 0x01 物体追踪指令
        - 0x02 面部表情识别
        - 0x03 车辆有无运动指令
    - adden2=:
        - 0x00 不使用该数据
        - 0x01 面部表情：积极
        - 0x02 面部表情：消极
        - 0x03 面部表情：中性
        - 0x04 车辆处于运动状态
        - 0x04 车辆未处于运动状态
        - 0x05 所追踪物体超出视角范围

    **当一个视觉工作完成标志被发送时，OpenMV应结束一次视觉工作**

2. OpenMV 主动识别信息数据
    - State=0x03
    - adden1=:
        - 0x01 探测到人脸

3. 车辆控制帧
    - State=0x04
    - XData:车辆前后控制量 正数为前进
    - YData:车辆旋转控制量 正数为顺时针旋转