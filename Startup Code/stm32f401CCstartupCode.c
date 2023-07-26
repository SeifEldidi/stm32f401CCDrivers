/*Startup Code is used to initilize the Sram of the MCU and place the IVT in the flash memory */
/* 1] Init the IVT
*  2] disable All global Interrupts
*  3] Set the Stack pointer with the first stack location
*  4] Copy . data section from rom to .data section in RAM
*  5] init the bss with zeros (clear .bss)
*  6] Enable All global Interrupts 
*  7] Jump to main function
*/

/* Main Funciton to be called by startup Code */
extern void main();


/*variables used to copy .data section to RAM*/
extern _sdata;
extern _edata;
extern _sidata;
/*variables to init the .bss section with zeros*/
extern _sbss;
extern _ebss;
/*Stack pointer*/
extern _estack;

/*Set stack Pointer */
uint32_t * const MSP_ADD =(uint32_t * const) &_estack;

/* IVT vector Table Implementation */
void  Reset_Handler                 void()  
void  NMI_Handler                   void()  __attribute((allias("default_handler")));
void  HardFault_Handler             void()  __attribute((allias("default_handler")));
void  MemManage_Handler             void()  __attribute((allias("default_handler")));
void  BusFault_Handler              void()  __attribute((allias("default_handler")));
void  UsageFault_Handler            void()  __attribute((allias("default_handler")));
void  SVC_Handler                   void()  __attribute((allias("default_handler")));
void  DebugMon_Handler              void()  __attribute((allias("default_handler")));
void  PendSV_Handler                void()  __attribute((allias("default_handler")));
void  SysTick_Handler               void()  __attribute((allias("default_handler")));                                                            */
void  EXTI16_PVD_IRQHandler         void()  __attribute((allias("default_handler")));       /* EXTI Line 16 interrupt /PVD through EXTI line detection interrupt               */
void  TAMP_STAMP_IRQHandler         void()  __attribute((allias("default_handler")));      /* Tamper and TimeStamp interrupts through the EXTI line                           */
void  EXTI22_RTC_WKUP_IRQHandler    void()  __attribute((allias("default_handler"))); /* EXTI Line 22 interrupt /RTC Wakeup interrupt through the EXTI line              */
void  FLASH_IRQHandler              void()  __attribute((allias("default_handler")));             /* FLASH global interrupt                                                          */
void  RCC_IRQHandler                void()  __attribute((allias("default_handler")));               /* RCC global interrupt                                                            */
void  EXTI0_IRQHandler              void()  __attribute((allias("default_handler")));              /* EXTI Line0 interrupt                                                            */
void  EXTI1_IRQHandler              void()  __attribute((allias("default_handler")));             /* EXTI Line1 interrupt                                                            */
void  EXTI2_IRQHandler              void()  __attribute((allias("default_handler")));             /* EXTI Line2 interrupt                                                            */
void  EXTI3_IRQHandler              void()  __attribute((allias("default_handler")));             /* EXTI Line3 interrupt                                                            */
void  EXTI4_IRQHandler              void()  __attribute((allias("default_handler")));             /* EXTI Line4 interrupt                                                            */
void  EXTI9_5_IRQHandler            void()  __attribute((allias("default_handler")));        /* EXTI Line[9:5] interrupts                                                       */
void  TIM1_BRK_TIM9_IRQHandler      void()  __attribute((allias("default_handler")));     /* TIM1 Break interrupt and TIM9 global interrupt                                  */
void  TIM1_UP_TIM10_IRQHandler      void()  __attribute((allias("default_handler")));    /* TIM1 Update interrupt and TIM10 global interrupt                                */
void  TIM1_TRG_COM_TIM11_IRQHandler void()  __attribute((allias("default_handler"))); /* TIM1 Trigger and Commutation interrupts and TIM11 global interrupt              */
void  TIM1_CC_IRQHandler            void()  __attribute((allias("default_handler"))); /* TIM1 Capture Compare interrupt                                                  */
void  TIM2_IRQHandler               void()  __attribute((allias("default_handler"))); /* TIM2 global interrupt                                                           */
void  TIM3_IRQHandler               void()  __attribute((allias("default_handler"))); /* TIM3 global interrupt                                                           */
void  TIM4_IRQHandler               void()  __attribute((allias("default_handler"))); /* TIM4 global interrupt                                                           */
void  I2C1_EV_IRQHandler            void()  __attribute((allias("default_handler"))); /* I2C1 event interrupt                                                            */
void  I2C1_ER_IRQHandler            void()  __attribute((allias("default_handler"))); /* I2C1 error interrupt                                                            */
void  I2C2_EV_IRQHandler            void()  __attribute((allias("default_handler"))); /* I2C2 event interrupt                                                            */
void  I2C2_ER_IRQHandler            void()  __attribute((allias("default_handler"))); /* I2C2 error interrupt                                                            */
void  SPI1_IRQHandler               void()  __attribute((allias("default_handler"))); /* SPI1 global interrupt                                                           */
void  SPI2_IRQHandler               void()  __attribute((allias("default_handler"))); /* SPI2 global interrupt                                                           */
void  USART1_IRQHandler             void()  __attribute((allias("default_handler"))); /* USART1 global interrupt                                                         */
void  USART2_IRQHandler             void()  __attribute((allias("default_handler"))); /* USART2 global interrupt                                                         */
void  EXTI15_10_IRQHandler          void()  __attribute((allias("default_handler"))); /* EXTI Line[15:10] interrupts                                                     */
void  EXTI17_RTC_Alarm_IRQHandler   void()  __attribute((allias("default_handler"))); /* EXTI Line 17 interrupt / RTC Alarms (A and B) through EXTI line interrupt       */
void  EXTI18_OTG_FS_WKUP_IRQHandler void()  __attribute((allias("default_handler"))); /* EXTI Line 18 interrupt / USBUSB On-The-Go FS Wakeup through EXTI line interrupt */
void  SDIO_IRQHandler               void()  __attribute((allias("default_handler"))); /* SDIO global interrupt                                                           */
void  TIM5_IRQHandler               void()  __attribute((allias("default_handler"))); /* TIM5 global interrupt                                                           */
void  SPI3_IRQHandler               void()  __attribute((allias("default_handler"))); /* SPI3 global interrupt                                                           */
void  OTG_FS_IRQHandler             void()  __attribute((allias("default_handler"))); /* USB On The Go FS global interrupt                                               */
void  USART6_IRQHandler             void()  __attribute((allias("default_handler"))); /* USART6 global interrupt                                                         */
void  I2C3_EV_IRQHandler            void()  __attribute((allias("default_handler"))); /* I2C3 event interrupt                                                            */
void  I2C3_ER_IRQHandler            void()  __attribute((allias("default_handler"))); /* I2C3 error interrupt                                                            */
void  FPU_IRQHandler                void()  __attribute((allias("default_handler"))); /* Floating point interrupt                                                        */
void  SPI4_IRQHandler               void()  __attribute((allias("default_handler"))); /* SPI4 global interrupt  */

uint32_t  * Int_VectorTable[] __attribute__((section("isr_vector"))) ={
        (uint32_t *)  MSP_ADD,
        (uint32_t *) _estack,
        (uint32_t *) Reset_Handler,
        (uint32_t *) NMI_Handler,
        (uint32_t *) HardFault_Handler,
        (uint32_t *) MemManage_Handler,
        (uint32_t *) BusFault_Handler,
        (uint32_t *) UsageFault_Handler,
        (uint32_t *) 0,
        (uint32_t *) 0,
        (uint32_t *) 0,
        (uint32_t *) 0,
        (uint32_t *) SVC_Handler,
        (uint32_t *) DebugMon_Handler,
        (uint32_t *) 0,
        (uint32_t *) PendSV_Handler,
        (uint32_t *) SysTick_Handler,
        (uint32_t *) 0              ,               /* Reserved                                                                        */
        (uint32_t *) EXTI16_PVD_IRQHandler  ,       /* EXTI Line 16 interrupt /PVD through EXTI line detection interrupt               */
        (uint32_t *) TAMP_STAMP_IRQHandler   ,      /* Tamper and TimeStamp interrupts through the EXTI line                           */
        (uint32_t *) EXTI22_RTC_WKUP_IRQHandler    , /* EXTI Line 22 interrupt /RTC Wakeup interrupt through the EXTI line              */
        (uint32_t *) FLASH_IRQHandler ,             /* FLASH global interrupt                                                          */
        (uint32_t *) RCC_IRQHandler ,               /* RCC global interrupt                                                            */
        (uint32_t *) EXTI0_IRQHandler,              /* EXTI Line0 interrupt                                                            */
        (uint32_t *) EXTI1_IRQHandler ,             /* EXTI Line1 interrupt                                                            */
        (uint32_t *) EXTI2_IRQHandler ,             /* EXTI Line2 interrupt                                                            */
        (uint32_t *) EXTI3_IRQHandler ,             /* EXTI Line3 interrupt                                                            */
        (uint32_t *) EXTI4_IRQHandler ,             /* EXTI Line4 interrupt                                                            */
        (uint32_t *) 0            ,                 /* Reserved                                                                        */
        (uint32_t *) 0            ,                 /* Reserved                                                                        */
        (uint32_t *) 0            ,                 /* Reserved                                                                        */
        (uint32_t *) 0            ,                 /* Reserved                                                                        */
        (uint32_t *) 0             ,                /* Reserved                                                                        */
        (uint32_t *) 0              ,               /* Reserved                                                                        */
        (uint32_t *) 0               ,              /* Reserved                                                                        */
        (uint32_t *) ADC_IRQHandler   ,             /* ADC1 global interrupt                                                           */
        (uint32_t *) 0                 ,            /* Reserved                                                                        */
        (uint32_t *) 0                  ,           /* Reserved                                                                        */
        (uint32_t *) 0                    ,         /* Reserved                                                                        */
        (uint32_t *) 0                   ,          /* Reserved                                                                        */
        (uint32_t *) EXTI9_5_IRQHandler    ,        /* EXTI Line[9:5] interrupts                                                       */
        (uint32_t *) TIM1_BRK_TIM9_IRQHandler ,     /* TIM1 Break interrupt and TIM9 global interrupt                                  */
        (uint32_t *) TIM1_UP_TIM10_IRQHandler  ,    /* TIM1 Update interrupt and TIM10 global interrupt                                */
        (uint32_t *) TIM1_TRG_COM_TIM11_IRQHandler, /* TIM1 Trigger and Commutation interrupts and TIM11 global interrupt              */
        (uint32_t *) TIM1_CC_IRQHandler           , /* TIM1 Capture Compare interrupt                                                  */
        (uint32_t *) TIM2_IRQHandler              , /* TIM2 global interrupt                                                           */
        (uint32_t *) TIM3_IRQHandler              , /* TIM3 global interrupt                                                           */
        (uint32_t *) TIM4_IRQHandler              , /* TIM4 global interrupt                                                           */
        (uint32_t *) I2C1_EV_IRQHandler           , /* I2C1 event interrupt                                                            */
        (uint32_t *) I2C1_ER_IRQHandler           , /* I2C1 error interrupt                                                            */
        (uint32_t *) I2C2_EV_IRQHandler           , /* I2C2 event interrupt                                                            */
        (uint32_t *) I2C2_ER_IRQHandler           , /* I2C2 error interrupt                                                            */
        (uint32_t *) SPI1_IRQHandler              , /* SPI1 global interrupt                                                           */
        (uint32_t *) SPI2_IRQHandler              , /* SPI2 global interrupt                                                           */
        (uint32_t *) USART1_IRQHandler            , /* USART1 global interrupt                                                         */
        (uint32_t *) USART2_IRQHandler            , /* USART2 global interrupt                                                         */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) EXTI15_10_IRQHandler         , /* EXTI Line[15:10] interrupts                                                     */
        (uint32_t *) EXTI17_RTC_Alarm_IRQHandler  , /* EXTI Line 17 interrupt / RTC Alarms (A and B) through EXTI line interrupt       */
        (uint32_t *) EXTI18_OTG_FS_WKUP_IRQHandler, /* EXTI Line 18 interrupt / USBUSB On-The-Go FS Wakeup through EXTI line interrupt */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) SDIO_IRQHandler              , /* SDIO global interrupt                                                           */
        (uint32_t *) TIM5_IRQHandler              , /* TIM5 global interrupt                                                           */
        (uint32_t *) SPI3_IRQHandler              , /* SPI3 global interrupt                                                           */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                             /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) OTG_FS_IRQHandler            , /* USB On The Go FS global interrupt                                               */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) USART6_IRQHandler            , /* USART6 global interrupt                                                         */
        (uint32_t *) I2C3_EV_IRQHandler           , /* I2C3 event interrupt                                                            */
        (uint32_t *) I2C3_ER_IRQHandler           , /* I2C3 error interrupt                                                            */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) FPU_IRQHandler               , /* Floating point interrupt                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) 0                            , /* Reserved                                                                        */
        (uint32_t *) SPI4_IRQHandler              , /* SPI4 global interrupt                                                           */
}

void SystemInit()
{

}

void  Reset_Handleroid()
{
    /* Variables Initilization */
    uint32_t MemSize = &_edata - &_sdata;
    uint32_t MemCounter = 0;
    uint32_t *SrcAddress = ((uint32_t *)(&_sdata));
    uint32_t *DstAddress = ((uint32_t *)(&_sidata));
    /*Copy .data[FLASH] -> .data[SRAM] */
    for( MemCounter = 0 ; MemCounter <= MemSize-1;MemCounter ++)
        *DstAddress++ = *SrcAddress++;
    /*Init Bss section with zeros */
    DstAddress = ((uint32_t *) &_sbss);
    MemSize    = &_ebss - &_sbss;
    for( MemCounter = 0 ; MemCounter <= MemSize-1;MemCounter ++)
        *DstAddress++ = 0
    /*Call SystemInit()*/
    SystemInit();
    /*Jmp to Main Function*/
    main();
}

void default_handler()
{

}

