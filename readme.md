# Linux 系统编程练习

### 一、多进程复制文件
代码在 `cp` 目录下，已全部完成
1. 指定创建子进程的个数
2. 打开源文件
3. 打开目的文件, 不存在则创建
4. 获取文件大小
5. 根据文件大小拓展目标文件
6. 为源文件创建映射
7. 为目标文件创建映射
8. 求出每个子进程该拷贝的字节数
9. 创建N个子进程
10. 子进程完成分块拷贝(注意最后一个子进程拷贝起始位置)
11. 释放映射区

使用方法：
```shell
cd ./cp     # cd到cp目录下面

make        # 编译，得到可执行文件 main

./main [childprocessNum] [srcFilename]  # 运行可执行文件，得到 copy.txt 的复制文件
```


### 二、简易shell
代码在 `easy_shell`目录下，实现步骤： 
1. 接收用户输入命令字符串，拆分命令及参数存储。（已完成)
2. 实现普通命令加载功能 （已完成)
3. 实现输入、输出重定向的功能 （已完成)
4. 实现管道 （已完成)
5. 支持多重管道（已完成)

使用方法：

```shell
cd ./cp     # cd到cp目录下面

make        # 编译，得到可执行文件 main

./main      # 运行可执行文件

# 示例命令
tree ../ > test.txt | cat < test.txt | grep main    
```
结果如下所示：

![result](https://user-images.githubusercontent.com/32126755/128508953-a2d335af-a2ce-4891-933a-ba3a0ed12b52.png)


### 三、本地聊天室
代码在 `local-chat` 文件夹下，实现步骤：
#### 1. 客户端
 1. 获取自己的 `room ID` 和 `friend's room ID`，填充 `MyProtocal` 结构体；

 2. 非阻塞属性创建私有FIFO；

 3. 打开服务器公共的FIFO,把自己的信息向服务器注册，私有FIFO的名称存储在结构体的 `data` 部分；

 4. 非阻塞读私有FIFO

	read返回-1，继续轮询

	read返回>0, 协议包解析

	- pro_num为1，代表登陆请求包      (发向服务器)

	- pro_num为2，代表连接成功包      (从服务器接收)

    - pro_num为3，代表数据发送包      (发向服务器) | (从服务器接收)

    - pro_num为4，代表客户不在线      (从服务器接收)

    - pro_num为5，代表退出登陆        (发向服务器)

 5. 设置标准输入为非阻塞属性，读标准输入

	read返回-1，继续轮询

	read返回>1,字符串解析，填充struct c_msg

	向服务器公共FIFO,写入msg,pro_num为3

#### 2. 服务器端
1. 打开公共fifo，阻塞等待读
    
2. 读到内容字符串解析
    
3. 判断协议号num

	1. 用户注册

		非阻塞写打开新用户创建的私有FIFO

		增加在线列表节点
            
	2. 用户聊天

		取出dst

		中转数据给客户
            
    3. 用户退出

        将用户从在线列表中摘除

        关闭用户私有FIFO
            
4. 返回第一步1,继续阻塞在公共FIFO读等待

#### 3. 编译命令

```bash
cd ./local-chat/

g++ client.cpp src/MyClient.cpp -I include/ -o client

g++ server.cpp src/MyServer.cpp -I include/ -o server
```

#### 4. 未完善功能
- 客户端退出chat界面，虽然仍有连接，但不会显示收到的消息；
- 服务器端没有打印输出信息；
- 服务器端未设置输入特定字符结束，只能 `ctrl + c` 结束进程；
- chat界面的信息显示分布不合理。

懒的改了，等心情好的时候。。。