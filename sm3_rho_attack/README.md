# SM3 Rho攻击
通过Rho方法来构造碰撞环，此处使用的构造函数为f(x) = x^2 + 1。
每次生成的新哈希值与之前生成的比较，首先保证此时的x与之前的x没有重复，然后若发现此哈希值与已保存哈希值重复，则代表发现一个碰撞。
测试构造前8bit碰撞结果如下：
![image](https://github.com/lumingfeifei0/homework-for-security/blob/main/sm3_rho_attack/capture_20220729164035809.jpg)
