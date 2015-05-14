//设置文件

////////////////////////////////////////////////////////////////////////////
//
//							XML 解析设定
//
///////////////////////////////////////////////////////////////////////////

#define SETTING_XML_MODE_TINYXML 0
#define SETTING_XML_MODE_MINXML 1
#define SETTING_XML_MODE_PUGIXML 2

extern int SETTING_XML_MODE;//XML解析模式。0-tinyxml 1-极小化解析 2-pugixml

extern int SETTING_XML_CACHE;//是否采用缓存，类似于OPCACHE一样，把解析好的XML以二进制的方式缓存到文件里面

extern int SETTING_XML_TESTCASE;//测试用例


///////////////////////////////////////////////////////////////////////////
//
//								画图设定
//
//////////////////////////////////////////////////////////////////////////

//全部使用GDI进行绘图，受限于内核效率
#define SETTING_DRAW_MODE_GDI 1
//全部在内存中进行绘图
#define SETTING_DRAW_MODE_MEMORY 2
//GDI缓存绘图
#define SETTING_DRAW_MODE_MEMGDI 3
//使用OpenGL进行绘图
#define SETTING_DRAW_MODE_OPENGL 4

//默认绘图模式为内存绘图
extern int SETTING_DRAW_MODE;

//绘图默认线程数量
extern int SETTING_DRAW_THREAD;

//是否绘图的对象是已经裁剪之后的
extern int SETTING_DRAW_CUTTING;

//////////////////////////////////////////////////////////////////////////
//
//								裁剪设定
//
/////////////////////////////////////////////////////////////////////////

//进行线段裁剪
#define SETTING_CUTTING_MODE_CALC 1
//用蒙版过滤像素
#define SETTING_CUTTING_MODE_FILTER 2

//默认裁剪模式为像素过滤
extern int SETTING_CUTTING_MODE;

//裁剪的时候线程数量
extern int SETTING_CUTTING_THREAD;

//NVIDIA® 的 CUDA™ 显卡加速计算
extern int SETTING_ENABLE_CUDA;

//计算三角函数等的时候是否启动打表
extern int SETTING_ENABLE_FUNCTION_TABLE;


//////////////////////////////////////////////////////////////////////////
//
//								设定读取
//
/////////////////////////////////////////////////////////////////////////

int loadIniSetting();