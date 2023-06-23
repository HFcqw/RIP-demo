# RIP-demo
## RIP路由选择协议的C语言模拟实现
### 一、设计题目：距离矢量路由选择算法的模拟实现
### 二、设计要求
#### （1）掌握距离矢量路由选择选择算法的工作原理。
#### （2）采用C、Java等编程语言实现距离矢量路由选择选择算法。
#### 算法输入：
![带权图G](output/RIP-图例.jpg)
#### 算法输出：
#### （1）某个顶点v（路由器）在第i次周期后的整个路由表。
#### （2）路由表的结构<目的网络，跳数，下一跳>
### 三、设计思路
距离矢量路由选择选择算法有多种，本设计采用的是比较经典的RIP协议，根据设计要求，只要求输出路由器在第i次周期后的整个路由表，为突出设计要求，本设计简化了RIP协议，与RIPv1，RIPv2和RIPng相比，本设计精简了水平分割、毒性逆转和抑制计时功能，本设计采用手动触发更新，即：w 系统中涉及到三个对象，network，router，routertable，分别表示网络空间，路由器，路由表。其中，系统一共具有1个网络，网络中包括至少一个路由器，每个路由器拥有一个路由表。 系统执行大致分为三个阶段：

#### 阶段一，系统读取配置文件./config/network.properties，通过其中的配置项来初始化网络空间，建立路由表，建立邻接关系。
#### 阶段二，系统提示用户输入周期i，用户将输入反馈给系统。
#### 阶段三，系统根据用户输入，执行响应轮次的路由表更新，输出结果。

#### 配置文件书写规则：
>e.g.\
>routernum=6 &ensp; //路由器数量\
>networknum=7 &ensp; //网络数量\
>0=0 &ensp; //左侧表示路由器编号，右侧表示网络编号\
>1=0,1,3,4 &ensp; //路由器和网络号都从0开始计数\
>2=1,2 &ensp; //右侧为该路由器连接的网络，中间用英文","隔开\
>3=3,6\
>4=3\
>5=4,5

### 四、动态路由 RIP 产生背景
1、动态路由协议包括距离向量路由协议和链路状态路由协议。RIP（Routing Information Protocols，路由信息协议）是使用最广泛的距离向量路由协议。RIP 是为小型网络环境设计的，因为这类协议的路由学习及路由更新将产生较大的流量，占用过多的带宽。\
2、RIP 是由 Xerox 在 70 年代开发的，最初定义在 RFC1058 中。RIP 用两种数据包传输更新：更新和请求，每个有 RIP 功能的路由器默认情况下每隔 30 秒利用 UDP 520 端口向与它直连的网络邻居广播（RIP v1）或组播（RIP v2）路由更新。因此路由器不知道网络的全局情况，如果路由更新在网络上传播慢，将会导致网络收敛较慢，造成路由环路。为了避免路由环路，RIP 采用水平分割、毒性逆转、定义最大跳数、闪式更新、抑制计时 5 个机制来避免路由环路。
RIP 协议分为版本 1 和版本 2。不论是版本 1 或版本 2，都具备下面的特征：
1. 是距离向量路由协议；
2. 使用跳数（Hop Count）作为度量值；
3. 默认路由更新周期为 30 秒；
4. 管理距离（AD）为 120；
5. 支持触发更新；
6. 最大跳数为 15 跳；
7. 支持等价路径, 默认 4 条, 最大 6 条；
8. 使用 UDP520 端口进行路由更新。
### 五、RIPv1 和 RIPv2 的区别
RIPv1 RIPv2在路由更新的过程中不携带子网信息 
在路由更新的过程中携带子网信息
不提供认证 提供明文和 MD5 认证
不支持 VLSM 和 CIDR 支持 VLSM 和 CIDR
采用广播更新 采用组播（224.0.0.9）更新
有类别（Classful）路由协议 无类别（Classless）路由协议
