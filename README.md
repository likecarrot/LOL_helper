##### RIOT_LCU项目是一个基于网易duilib库+Riot API制作的lol自动接受对局的程序

各位使用者可以观看来自bilibili的视频演示,提供的功能基于Riot API,不对游戏内存进行任何操作.

###### 提供功能:

1.自动接受对局.

2.打完一局之后自动再来一局.

3.在房间内自动开始匹配.

4.自动锁定英雄,仅测试人机模式下,自定义及训练模式无效.

###### 涉及到的技术:

1.网易duilib库

2.windows消息循环

3.C++网络请求

4.一些windows下编程技术,如使用互斥量确认进程唯一,使用wmic获取进程启动命令.