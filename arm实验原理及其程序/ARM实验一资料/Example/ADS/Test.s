        AREA example, CODE, READONLY     ;���������example
        ENTRY							 ;��ʶ�������
        CODE32							 ;����Ϊ32λARMָ��
	 
START
        MOV     R0, #15					 ;R0 �� 15
        MOV     R1, #8					 ;R1 �� 8
        ADDS    R0, R0, R1				 ;R0 �� R0��R1
        
        B       START					 ;����START���¿�ʼ����
        
        END 							 