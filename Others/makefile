boot.bat: boot.asm
	nasm -o boot.bat boot.asm
cp: ckernel_u.asm
	cp ../kernel/kernel.asm .
	cp ../boot/boot.asm .
	cp ../include/pm.inc .
	cp ../include/fontData.inc .
rm: ckernel_u.asm
	rm -f *.o
ckernel_u.asm : ckernel.o
	./objconv -fnasm ckernel.o ckernel_u.asm
ckernel.o : main.o window.o mem_manage.o timer.o global_define.o
	ld -m elf_i386 -r main.o mem_manage.o window.o timer.o global_define.o -o ckernel.o
main.o : ../graphic/Main.c ../graphic/window.c ../memory/mem_manage.c ../kernel/global_define.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o main.o ../graphic/Main.c -I ../include/
window.o : ../graphic/window.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o window.o ../graphic/window.c -I ../include/
mem_manage.o : ../memory/mem_manage.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o mem_manage.o ../memory/mem_manage.c -I ../include/
timer.o : ../kernel/timer.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o timer.o ../kernel/timer.c -I ../include/
global_define.o: ../kernel/global_define.c
	gcc -m32 -fno-asynchronous-unwind-tables -s -c -o global_define.o ../kernel/global_define.c -I ../include/
