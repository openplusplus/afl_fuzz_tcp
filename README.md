# afl_fuzz_tcp
用afl-fuzz测试网络程序


## 如何使用
1 建立一个新的in目录，目录中放入写好的测试用例。（mkdir in out）
2 用afl-g++ 编译好afl_fuzz_tcp程序
3 运行：afl-fuzz -i in -o out ./afl_fuzz_tcp @@

这样程序就会从in文件中读出测试用例，然后afl-fuzz自动生成测试用例，然后开始测试了
