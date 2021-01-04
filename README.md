# 项目说明
- 这是一个用TTL与非门电路自制CPU项目
- 微指令结构
- 8位数据总线
- 8位地址总线
- 9个8位寄存器(IR,IF,IP,SP,AR,BR,CR,DR,TR)
- 1个8位ALU
- 目前只用Proteus做模拟

# 项目文件
|文件|说明|
|----|----|
|8bit-cpu.dsn|Proteus的模拟|
|8bit-cpu.LYT|Proteus的PCB|
|16bit-cpu.dsn|Proteus的模拟|
|doc/alu.png|ALU74ls181逻辑功能表|
|doc/cpu.png|cpu逻辑架构图|
|doc/make pcb.txt|将ares转为CAM350步骤|
|doc/mi.xls|微指令详细|
|mi/mi.exe|生成微指令工具,将mi.txt转化成mi_*.bin|
|mi/MI.c|mi.exe的源代码|
|mi/mi.txt|微指令源码|
|mi/mi_*.bin|微指令|
|mi/rom.bin|内存程序|

# 逻辑结构
![cmd-markdown-logo](https://github.com/xt9852/CPU/blob/master/doc/cpu.png)

# 物理结构
- 微指令驱动部分
    由MINA0-1,MIA0-1组成,
    由IN-CLK,CLK_信号驱动,
    主要是对微指令的当前地址进行操作

- 微指令跳转部分
    由SEL0-5组成,
    由IN-INT0-1,CHK-IS,JL,JNE信号生成的SEL-A,SEL-B信号驱动,
    主要是对微指令的下一条地址进行操作,
    通过选择不同的微指令地址以实现微指令跳转

    |SEL-B|SEL-A|说明|
    |----|----|----|
    |0|0|选取下一条地址为微指令地址|
    |0|1|选取0x[IR]0组成微指令地址|
    |1|0|选取中断微指令地址0x005|
    |1|1|选取条件跳转微指令地址0x004|
    
- 微指令存储部分
    由MI0~7组成，
    由MIA[]驱动

    |微指令|说明|
    |----|----|
    |CHK-IR|选取指令寄存器做微指令地址|
    |CHK-INT|检测是否发生中断|
    |CHK-JE|检测是否等于跳转|
    |CHK-JNE|检测是否不等于跳转|
    |CHK-JB|检测是否大于跳转|
    |CHK-JBN|检测是否大于等于跳转|
    |CHK-JL|检测是否小于跳转|
    |CHK-JLN|检测是否小于等于跳转|
    |AR-A|连通A总线,0-输入,1-输出|
    |AR-RW|操作:0-读,1写|
    |AR-D|连通D总线,0-输入,1-输出|
    |ALU-S0|ALU控制S0|
    |ALU-S1|ALU控制S1|
    |ALU-S2|ALU控制S2|
    |ALU-S3|ALU控制S3|
    |ALU-M|ALU运算类型:0-算数,1-逻辑|
    |ALU-CN|ALU进位|
    |M-CE|内存选通|
    |M-RW|内存操作:0-读，1写|
    |M-OE|内存读操作:0-有效，1-无效|
    |M-WE|内存写操作:0-有效，1-无效|

- 寄存器与总线部分
    IR,IR-D,IF,IF-A,IF-D,IP,IP-A,IP-D,SP,SP-A,SP-D,
    AR,AR-A,AR-D,BR,BR-A,BR-D,CR,CR-A,CR-D,DR,DR-A,DR-D,
    TR_F,TR,TR_D

- ALU与临时寄存器部分
    由ALU0-1，TR组成,
    由ALU-S0-3,M,CN,TR_F信号驱动

- 内存部分
    由ROM,RAM,M0-1组成,
    由ABus,M-CE,RW,OE,WE信号驱动

# 所用芯片
|芯片|说明|
|----|----|
|74LS04|非门|
|74LS08|2输入与门|
|74LS32|2输入或门|
|74LS138|3-8译码器|
|74LS153|双4选1选择器|
|74LS165|并入串出|
|74LS181|逻辑运算器|
|74LS245|双向驱动器|
|74LS373|寄存器|
|2732|ROM存储器,存储微指令|
|27256|ROM存储器|
|62256|RAM存储器|

# 数据流
- 微指令跳转部分=>微指令驱动部分=>微指令存储部分=>寄存器与总线部分,ALU与临时寄存器部分,内存部分

# 指令执行过程
- 清空各寄存器操作
- 检测中断操作
- 取指和IP++操作
- 更新IP操作
- 执行指令操作

# 微代码说明
- [微代码详细](https://github.com/xt9852/CPU/blob/master/doc/mi.xls)

# ALU逻辑
|S0|S1|S2|S3|M|Cn|功能|
|----|----|----|----|----|----|----|
|H|H|L|L|H|*|0|
|L|L|H|H|H|*|1|
|H|H|H|H|H|*|A|
|H|L|H|L|H|*|B|
|L|L|L|L|H|*|A非|
|H|H|L|H|H|*|A与B|
|L|H|H|H|H|*|A或B|
|L|H|H|L|H|*|A异或B|
|H|L|L|H|L|H|A加B|
|L|H|H|L|L|L|A减B|
|L|L|L|L|L|L|A++|
|H|H|H|H|L|H|A--|


