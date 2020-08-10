CFLAGS=-fno-stack-protector
boot.bat: boot.asm
	nasm -o boot.bat boot.asm
boot.asm: ckernel_u.asm rm
	cp ../kernel/kernel.asm .
	cp ../kernel/crack_call.asm .
	cp ../kernel/api_call.asm .
	cp ../boot/boot.asm .
	cp ../include/pm.inc .
	cp ../include/fontData.inc .
rm: ckernel_u.asm app_u.asm crack_u.asm
	rm -f *.o
ckernel_u.asm : ckernel.o
	./objconv -fnasm ckernel.o ckernel_u.asm
ckernel.o : write_vga_desktop.o win_sheet.o mem_util.o timer.o global_define.o multi_task.o
	ld -m elf_i386 -r write_vga_desktop.o mem_util.o win_sheet.o timer.o global_define.o multi_task.o  -o ckernel.o
write_vga_desktop.o : ../graphic/write_vga_desktop.c ../graphic/win_sheet.c ../memory/mem_util.c ../kernel/timer.c ../kernel/global_define.c ../process/multi_task.c
	gcc -m32  -fno-asynchronous-unwind-tables -fno-stack-protector  -s -c -o write_vga_desktop.o ../graphic/write_vga_desktop.c -I ../include/
win_sheet.o : ../graphic/win_sheet.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o win_sheet.o ../graphic/win_sheet.c -I ../include/
mem_util.o : ../memory/mem_util.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o mem_util.o ../memory/mem_util.c -I ../include/
timer.o : ../kernel/timer.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o timer.o ../kernel/timer.c -I ../include/
global_define.o: ../kernel/global_define.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o global_define.o ../kernel/global_define.c -I ../include/
multi_task.o : ../process/multi_task.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o multi_task.o ../process/multi_task.c -I ../include/
app_u.asm : app.o
	./objconv -fnasm  app.o app_u.asm
app.o : ../kernel/app.c
	gcc -m32 -fno-stack-protector -fno-asynchronous-unwind-tables -s -c -o app.o ../kernel/app.c
crack_u.asm : crack.o
	./objconv -fnasm crack.o crack_u.asm
crack.o : ../kernel/crack.c
	gcc -m32 -fno-stack-protector -fno-asynchronous-unwind-tables -s -c -o crack.o ../kernel/crack.c
