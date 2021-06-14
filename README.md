# HardwareAccelerator
5x5 Matrix Transpose using Hardware accelerator

The code for the user interface is design in Vitis 2020.2. 
The user inputs the elements of the 5x5 matrix in Vitis Terminal.
Once all the elements are input, they are sent to the FPGA board
(Nexys A7 - 100T) through UART communication integrated using
MicroBlaze. The hardware block design was created using the 
Vivado 2020.2 Intellectual Property blocks along with my 
matrixTranspose.v design. The focus was to take in one element
at a time. A more simple-minded design would have added an extra
GPIO port per element. This project was a challenge as one GPIO
was used to handle all the input elements to the hardware accelerator
instead of having one GPIO port per element. This way, the hardware
accelerator can have a more general use and can be tailored to any
sized matrix and any type of calculation involving matrices.
