            PRESERVE8 
            EXPORT SysTick_Handler   
            EXPORT PendSV_Handler 
            EXPORT SVC_Handler
            EXPORT HardFault_Handler
            EXPORT EnableFPU
            EXPORT upcall                  [WEAK]
            EXPORT vmstart

                
            AREA    |.text|, CODE, READONLY  
     IMPORT StackSwitchPSV   
     IMPORT VMTick
     IMPORT VMSvc                                         
     IMPORT VMInit
     IMPORT VMStart
     IMPORT VMHardFault
     IMPORT Reset_Handler
                 ENDP
upcall           FUNCTION
                    SWI     0x02
                    BX      LR
                    ENDP
vmstart           FUNCTION  
                    CPSID   I
                    ;MSR     MSP, R1 
                    DSB
                    BL      EnableFPU
                    BL      VMStart  
                    DMB
                    MSR     PSP, R0                     
                    ;MOV     R1, #6
                    MSR     CONTROL, R1
                    CPSIE   I
                    POP     {PC}
    
                 ENDP
SysTick_Handler  FUNCTION 
                    CPSID   I
                    DSB
                    MRS     R0, PSP
                    STMDB   R0!, {R4-R11}
                    PUSH    {LR}  
                    BL      VMTick
                    DMB
                    LDMIA   R0!, {R4-R11}
                    MSR     PSP, R0
                    MSR     CONTROL, R1
                    CPSIE   I
                    POP     {PC}
                ENDP  
PendSV_Handler  FUNCTION  

                ENDP  
SVC_Handler     FUNCTION      
                    CPSID   I
                    DSB
                    PUSH    {LR} 
                    MRS     R3, PSP
                    STMDB   R3!, {R4-R11}
                    BL      VMSvc
                    LDMIA   R0!, {R4-R11}
                    MSR     PSP, R0
                    MSR     CONTROL, R1
                    CPSIE   I
                    POP     {PC}
                ENDP 
                    
HardFault_Handler FUNCTION      
                    CPSID   I
                    DSB
                    PUSH    {LR} 
                    MRS     R3, PSP
                    STMDB   R3!, {R4-R11}
                    BL      VMHardFault
                    B       Reset_Handler
                    ;B       .
                    ;LDMIA   R0!, {R4-R11}
                    ;MSR     PSP, R0
                    ;MSR     CONTROL, R1
                    ;CPSIE   I
                    ;POP     {PC}
                ENDP 
TableEnd        
                
EnableFPU       FUNCTION
                    ; CPACR is located at address 0xE000ED88
                    LDR R0, =0xE000ED88
                    ; Read CPACR
                    LDR R1, [R0]
                    ; Set bits 20-23 to enable CP10 and CP11 coprocessors
                    ORR R1, R1, #(0xF << 20)
                    ; Write back the modified value to the CPACR
                    STR R1, [R0]; wait for store to complete
                    DSB
                    ;reset pipeline now the FPU is enabled
                    ISB
                    BX  LR
                ENDP   
                END
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                