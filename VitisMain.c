/*
Final Project -- 5x5 Matrix Transpose
CECS460 -- Pooria Yaghini
Computer Engineering Team
 - Justin Elassal
 - Tylor Cooks
 - Trevor Scott
 - Farhad Nourbaksh
*/

//MatMult
#include "xgpio.h"

//send data over UART
#include "xil_printf.h"

//receive data over UART
#include "xuartlite_l.h"
//Reset FIFO
#include "xuartlite.h"
//information about AXI peripherals
#include "xparameters.h"
#include <stdio.h>
#include "xtmrctr.h"



#define Gpio0ID XPAR_AXI_GPIO_0_DEVICE_ID //receiveIn --  5 bits
#define Gpio1ID XPAR_AXI_GPIO_1_DEVICE_ID //sendOut   --  5 bits
#define Gpio2ID XPAR_AXI_GPIO_2_DEVICE_ID //intIn     -- 32 bits
#define Gpio3ID XPAR_AXI_GPIO_3_DEVICE_ID //intOut    -- 32 bits
#define Gpio4ID XPAR_AXI_GPIO_4_DEVICE_ID //speed     -- 10 bits

#define TMRCTR_DEVICE_ID	XPAR_TMRCTR_0_DEVICE_ID
#define TIMER_COUNTER_0	 0

XTmrCtr TimerCounter; /* The instance of the Tmrctr Device */

//The following function begins the timer
initTimer(u16 DeviceId, u8 TmrCtrNumber){
	int Status;
		u32 Value1;
		u32 Value2;
		XTmrCtr *TmrCtrInstancePtr = &TimerCounter;

		/*
		 * Initialize the timer counter so that it's ready to use,
		 * specify the device ID that is generated in xparameters.h
		 */
		Status = XTmrCtr_Initialize(TmrCtrInstancePtr, DeviceId);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}

		/*
		 * Perform a self-test to ensure that the hardware was built
		 * correctly, use the 1st timer in the device (0)
		 */
		Status = XTmrCtr_SelfTest(TmrCtrInstancePtr, TmrCtrNumber);
		if (Status != XST_SUCCESS) {
			return XST_FAILURE;
		}


		/*
		 * Enable the Autoreload mode of the timer counters.
		 */
		XTmrCtr_SetOptions(TmrCtrInstancePtr, TmrCtrNumber,
					XTC_AUTO_RELOAD_OPTION);

		/*
		 * Get a snapshot of the timer counter value before it's started
		 * to compare against later
		 */
		Value1 = XTmrCtr_GetValue(TmrCtrInstancePtr, TmrCtrNumber);

		/*
		 * Start the timer counter such that it's incrementing by default
		 */
		XTmrCtr_Start(TmrCtrInstancePtr, TmrCtrNumber);
		/*
		 * Read the value of the timer counter and wait for it to change,
		 * since it's incrementing it should change, if the hardware is not
		 * working for some reason, this loop could be infinite such that the
		 * function does not return
		 */
		while (1) {
			Value2 = XTmrCtr_GetValue(TmrCtrInstancePtr, TmrCtrNumber);
			if (Value1 != Value2) {
				break;
			}
		}
}
//Stops the Timer and prints out the last value of the counter
endTimer(u16 DeviceId, u8 TmrCtrNumber){
	XTmrCtr *TmrCtrInstancePtr = &TimerCounter;
	int Value2 = XTmrCtr_GetValue(TmrCtrInstancePtr, TmrCtrNumber);
	xil_printf("TotalTime: %d ns\n", Value2);
	/*
	 * Disable the Autoreload mode of the timer counters.
	 */
	XTmrCtr_SetOptions(TmrCtrInstancePtr, TmrCtrNumber, 0);
}

void writeMatrix(int* element){
	int i;
	for(i=0;i<25;i++){
		*(element+i) = 0;
		xil_printf("\rElement %d =  ",i);
		*(element+i) += (XUartLite_RecvByte (XPAR_AXI_UARTLITE_1_BASEADDR)-48)*10;
		*(element+i) += (XUartLite_RecvByte (XPAR_AXI_UARTLITE_1_BASEADDR)-48);
		xil_printf("%d",*(element+i));
		while (XUartLite_RecvByte (XPAR_AXI_UARTLITE_1_BASEADDR)!=13);
	}
	xil_printf("\rMatrix Received!");
}


int main()
{
	//XGpio gpio0, gpio1,gpio2,gpio3;
	XGpio receiveIn,sendOut,intIn,intOut,speed;
	int i;
	//initialize AXI GPIO
	XGpio_Initialize(&receiveIn, Gpio0ID);
	XGpio_Initialize(&sendOut, Gpio1ID);
	XGpio_Initialize(&intIn  , Gpio2ID);
	XGpio_Initialize(&intOut , Gpio3ID);
	XGpio_Initialize(&speed  , Gpio4ID);


	//Set the direction of AXI GPIO

	XGpio_SetDataDirection(&receiveIn,1, 0x00000000); // set receiveIn as output from Vitis
	XGpio_SetDataDirection(&sendOut,  1, 0x00000000); // set sendOut   as output from Vitis
	XGpio_SetDataDirection(&intIn,    1, 0x00000000); // set intIn     as output from Vitis
	XGpio_SetDataDirection(&intOut,   1, 0xFFFFFFFF); // set intOut    as  input   to Vitis
	XGpio_SetDataDirection(&speed,    1, 0xFFFFFFFF); // set speed     as  input   to Vitis
	//Change Values Here
	int matA[25];
	int* matPtr = &matA[0];
	int matB[25];
	writeMatrix(matPtr);//creates matrix that needs to be transposed

	
	for(i=1;i<26;++i){
		XGpio_DiscreteWrite(&receiveIn, 1, i);     //Vivado will receive integer
		XGpio_DiscreteWrite(&intIn, 1, matA[i-1]);//load integer that will be sent to FPGA
	}


	//Begin PL timer**********************************************************
		initTimer(TMRCTR_DEVICE_ID, TIMER_COUNTER_0);
	for(i=1;i<26;i++){
		XGpio_DiscreteWrite(&sendOut, 1, i);
		matB[i-1] = XGpio_DiscreteRead(&intOut,1);
		//xil_printf(" %d ",matB[i]);
	}
	endTimer(TMRCTR_DEVICE_ID, TIMER_COUNTER_0);
	
	xil_printf("\nOriginal matrix below\n");
	xil_printf("\n");
	for(i=0;i<5;++i) xil_printf(" %d ",matA[i]);
	xil_printf("\n");
	for(i=5;i<10;++i) xil_printf(" %d ",matA[i]);
	xil_printf("\n");
	for(i=10;i<15;++i) xil_printf(" %d ",matA[i]);
	xil_printf("\n");
	for(i=15;i<20;++i) xil_printf(" %d ",matA[i]);
	xil_printf("\n");
	for(i=20;i<25;++i) xil_printf(" %d ",matA[i]);
	xil_printf("\n");
	
	xil_printf("\nTransposed matrix below\n");
	for(i=0;i<5;++i) xil_printf(" %d ",matB[i]);
	xil_printf("\n");
	for(i=5;i<10;++i) xil_printf(" %d ",matB[i]);
	xil_printf("\n");
	for(i=10;i<15;++i) xil_printf(" %d ",matB[i]);
	xil_printf("\n");
	for(i=15;i<20;++i) xil_printf(" %d ",matB[i]);
	xil_printf("\n");
	for(i=20;i<25;++i) xil_printf(" %d ",matB[i]);
	xil_printf("\n");

	while(1){}
}
