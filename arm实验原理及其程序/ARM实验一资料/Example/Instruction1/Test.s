
COUNT   EQU  0x40004100                 ;����һ����������ַΪ0x40004100
        
        
        AREA  Example, CODE, READONLY   ;���������Example
        ENTRY							;��ʶ�������
        CODE32							;����32λARMָ��
       
START   LDR     R1, =COUNT				;��count�ĵ�ֵַ����R1
        MOV     R0, #0					;R0 = 0
        STR     R0, [R1]    		    ;count = 0
        
        
LOOP    LDR     R1, =COUNT              
        LDR     R0, [R1]				;R0 = count
        ADD     R0, R0, #1				;R0 = R0 + 1
        CMP     R0, #10					;R0��10���бȽ�
        MOVHS   R0, #0					;��R0 > 10�����ָ��ִ��R0 �� 0
        STR     R0, [R1]				;��R0��ֵ���浽count��
        
        
        B       LOOP
        
        END            