        AREA example, CODE, READONLY     ;声明代码段example
        ENTRY							 ;标识程序入口
        CODE32							 ;声明为32位ARM指令
	 
START
        MOV     R0, #15					 ;R0 ＝ 15
        MOV     R1, #8					 ;R1 ＝ 8
        ADDS    R0, R0, R1				 ;R0 ＝ R0＋R1
        
        B       START					 ;跳到START重新开始运行
        
        END 							 