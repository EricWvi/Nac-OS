# Nac-OS

### Graphic
#### Main.c
* 





### kernel

* io_hlt() 计算机硬件暂停指令

* io_in8, io_in16, io_out8 写入读取端口
* io_cli 关闭中断 io_sti 允许中断
* io_load_eflags  io_store_eflags 保存恢复硬件功能 例如中断功能
* init_palette set_palette 初始化 设置 调色板
* `void boxfill8(unsigned char *vram,int xsize,  unsigned char c, int x, int y,int x0, int y0);` 
  * vram 显存 xsize屏幕x方向长度 x0y0左上角 xy左下角 c颜色
  * 示例
* putblock 二维数组转为一维写入显存



局部变量不能超过512字节 a[513] 不可以