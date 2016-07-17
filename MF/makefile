run : main.o mf.o ../Evaluation/recomm.o ../GeneralStruct/gvStruct.o
	gcc main.o mf.o ../Evaluation/recomm.o ../GeneralStruct/gvStruct.o -lm -O2 -o run
main.o : main.c main.h
	gcc main.c -c -O2 -std=c99
mf.o : mf.c mf.h
	gcc mf.c -c -O2 -std=c99
../Evaluation/recomm.o : ../Evaluation/recomm.c ../Evaluation/recomm.h
	gcc ../Evaluation/recomm.c -c -std=c99 -O2 -o ../Evaluation/recomm.o
../GeneralStruct/gvStruct.o : ../GeneralStruct/gvStruct.c ../GeneralStruct/gvStruct.h
	gcc ../GeneralStruct/gvStruct.c -c -std=c99 -O2 -o ../GeneralStruct/gvStruct.o 
clean:
	rm run *.o
