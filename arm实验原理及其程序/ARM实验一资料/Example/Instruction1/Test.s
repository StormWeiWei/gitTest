
COUNT   EQU  0x40004100                 ;定义一个变量，地址为0x40004100
        
        
        AREA  Example, CODE, READONLY   ;声明代码段Example
        ENTRY							;标识程序入口
        CODE32							;声明32位ARM指令
       
START   LDR     R1, =COUNT				;将count的地址值赋给R1
        MOV     R0, #0					;R0 = 0
        STR     R0, [R1]    		    ;count = 0
        
        
LOOP    LDR     R1, =COUNT              
        LDR     R0, [R1]				;R0 = count
        ADD     R0, R0, #1				;R0 = R0 + 1
        CMP     R0, #10					;R0与10进行比较
        MOVHS   R0, #0					;若R0 > 10，则此指令执行R0 ＝ 0
        STR     R0, [R1]				;将R0的值保存到count中
        
        
        B       LOOP
        
        END            