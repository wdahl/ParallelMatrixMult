run: Part_1.out Part_2_serial.out Part_2_parallel.out Part_3.out

Part_1.out: Part_1.c timing.c 
	gcc -g Part_1.c timing.c -o Part_1.out

Part_2_serial.out: Part_2_serial.c timing.c
	gcc -g Part_2_serial.c timing.c -o Part_2_serial.out

Part_2_parallel.out: Part_2_parallel.c timing.c
	gcc -g Part_2_parallel.c timing.c -o Part_2_parallel.out -lpthread

Part_3.out: Part_3.c timing.c
	gcc -g Part_3.c timing.c -o Part_3.out -lpthread