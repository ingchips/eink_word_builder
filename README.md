# E-ink 单词卡

## 编译方法

1. 将本项目（整个文件夹）放置于 "{SDK安装目录}/{my_apps}/" 目录下编译。
    {my_apps} 可以是任意名称。

1. 在 Wizard 里导入项目；

1. 在 Wizard 打开项目，按提示完成 SDK 版本迁移；

1. 编译。

## 目录说明

* _img\_gen_ 目录下为用来生成墨水屏图像的小工具。

* _prebuild_ 目录：

    * _img\_gen.exe_ 为编译过的小工具

        个别版本的 Windows Defender 可能会误报 "Win32/DefenseEvasion.A!ml"。可以通过源代码自行编译。

    * start.img, done.img

        参考图像。

详细信息：https://ingchips.github.io/blog/2022-08-13-eink-word-builder/

## 第三方代码

本程序使用了以下第三方代码，遵循各自协议：

* [Fatfs](http://elm-chan.org/fsw/ff/00index_e.html)
* [墨水屏驱动](https://gitee.com/openLuat/LuatOS/blob/master/components/epaper)


