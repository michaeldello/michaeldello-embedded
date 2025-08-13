    .syntax unified
    .cpu cortex-m33
    .thumb

    .section .isr_vector,"a",%progbits
    .type g_pfnVectors, %object
g_pfnVectors:
    .word estack            /* Initial stack ptr */
    .word Reset_Handler
    .word Default_Handler   /* NMI */
    .word Default_Handler   /* HardFault */
    .word Default_Handler   /* MemManage */
    .word Default_Handler   /* BusFault */
    .word Default_Handler   /* UsageFault */
    .word 0,0,0,0           /* Reserved */
    .word Default_Handler   /* SVC */
    .word Default_Handler   /* DebugMon */
    .word 0                 /* Reserved */
    .word Default_Handler   /* PendSV */
    .word Default_Handler   /* SysTick */
    /* Extend for peripherals ... */

    .text
    .thumb
    .align 2
    .global Reset_Handler
Reset_Handler:
    /* Copy .data from FLASH to RAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
1:
    cmp r0, r1
    ittt lt
    ldrlt r3, [r2], #4
    strlt r3, [r0], #4
    blt 1b

    /* Zero out bss */
    ldr r0, =_sbss
    ldr r1, =_ebss
    movs r0, #0
2:
    cmp r0, r1
    itt lt
    strlt r2, [r0], #4
    blt 2b

    /* Call SystemInit for clocks (optional) */
    /* bl SystemInit */

    bl main

    /* If main returns, loop forever */
3:  b 3b

    .weak Default_Handler
Default_Handler:
    b Default_Handler
