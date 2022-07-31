# SM3 长度扩展攻击

## 原理

对任意的一条消息r1，得到它的填充结果pad，其最终哈希值为x1。

攻击者进行攻击时挑选消息r2，通过计算pad||r2的哈希为x2；

把x1作为IV对r2做哈希，得到x3。

若x2与x3完全相等则代表长度扩展攻击成功

## 代码

在原先用C++实现的SM3代码上，添加函数lenth_attack_iteration(string str, string iv) ，实现把输入参数iv作为加密的IV。

lenth_attack函数，实现长度扩展攻击。

## 运行结果

运行结果如图：



