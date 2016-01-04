#include "init.h"
#include "gpio.h"

static void CPU_CACHE_Enable(void);
void SystemClock_Config(void);

static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x20010000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

void SystemClock_Config(void)
{
  __PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 216;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 216 MHz Frequency */  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4; 
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }  
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


void core_init ()
{
    HAL_Init();
    CPU_CACHE_Enable();
    MPU_Config();
}

void clock_enable ()
{
      SystemClock_Config();
      __GPIOA_CLK_ENABLE();
      __GPIOB_CLK_ENABLE();
      __GPIOC_CLK_ENABLE();
      __GPIOD_CLK_ENABLE();
      __GPIOH_CLK_ENABLE();
      __GPIOE_CLK_ENABLE();
      __GPIOI_CLK_ENABLE();
      __GPIOG_CLK_ENABLE();
      __GPIOH_CLK_ENABLE();
      //__ADC1_CLK_ENABLE();
      //__ADC2_CLK_ENABLE();
      //__DAC_CLK_ENABLE();
      __SPI4_CLK_ENABLE();
      //__DMA2_CLK_ENABLE();
      //__TIM1_CLK_ENABLE();
      //__TIM2_CLK_ENABLE();
}

void gpio_init ()
{
    gpio_alt_init(spi3_miso_port, spi3_miso_pin, GPIO_SPEED_MEDIUM, GPIO_MODE_AF_PP, GPIO_AF5_SPI4);
    gpio_alt_init(spi3_mosi_port, spi3_mosi_pin, GPIO_SPEED_MEDIUM, GPIO_MODE_AF_PP, GPIO_AF5_SPI4);
    gpio_alt_init(spi3_sck_port, spi3_sck_pin, GPIO_SPEED_MEDIUM, GPIO_MODE_AF_PP, GPIO_AF5_SPI4);
    gpio_pin_init(spi3_cs_port, spi3_cs_pin, GPIO_SPEED_MEDIUM, GPIO_MODE_OUTPUT_PP);
    gpio_pin_init(enc28j60_reset_port, enc28j60_reset_pin, GPIO_SPEED_HIGH, GPIO_MODE_OUTPUT_PP);
    gpio_pin_init(enc28j60_int_port, enc28j60_int_pin, GPIO_SPEED_HIGH, GPIO_MODE_INPUT);
    
    //gpio_pin_init(coil_out_port, coil_out_pin, GPIO_SPEED_HIGH, GPIO_MODE_OUTPUT_PP);
    
    gpio_pin_init(A_port, A_pin, GPIO_SPEED_HIGH, GPIO_MODE_OUTPUT_PP);
    gpio_pin_init(B_port, B_pin, GPIO_SPEED_HIGH, GPIO_MODE_OUTPUT_PP);
    gpio_pin_init(C_port, C_pin, GPIO_SPEED_HIGH, GPIO_MODE_OUTPUT_PP);
    gpio_pin_init(D_port, D_pin, GPIO_SPEED_HIGH, GPIO_MODE_OUTPUT_PP);
}

void spi_init ()
{
    
}

SPI_HandleTypeDef         SpiHandle_Enc28j60;

void SPI_Enc28j60_Init(void)
{    
    SpiHandle_Enc28j60.Instance               = SPI4;
    SpiHandle_Enc28j60.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    SpiHandle_Enc28j60.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle_Enc28j60.Init.CLKPhase          = SPI_PHASE_1EDGE;
    SpiHandle_Enc28j60.Init.CLKPolarity       = SPI_POLARITY_LOW;
    SpiHandle_Enc28j60.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    SpiHandle_Enc28j60.Init.CRCPolynomial     = 7;
    SpiHandle_Enc28j60.Init.DataSize          = SPI_DATASIZE_8BIT;
    SpiHandle_Enc28j60.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle_Enc28j60.Init.NSS               = SPI_NSS_SOFT;
    SpiHandle_Enc28j60.Init.TIMode            = SPI_TIMODE_DISABLE;
    SpiHandle_Enc28j60.Init.Mode              = SPI_MODE_MASTER;  
    SpiHandle_Enc28j60.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;

    //HAL_SPI_DeInit(&SpiHandle_Enc28j60);
    
    if(HAL_SPI_Init(&SpiHandle_Enc28j60) != HAL_OK)
    {

    }
    SET_BIT(SpiHandle_Enc28j60.Instance->CR2, SPI_RXFIFO_THRESHOLD);
    //HAL_SPI_Transmit(&SpiHandle_Enc28j60, test, 1, 10);
    __HAL_SPI_ENABLE(&SpiHandle_Enc28j60);
}
