chiellini: gfx3.o final.o finalfn.o
	gcc final.o gfx3.o finalfn.o -o chiellini -lX11 -lm

gfx3.o: gfx3.c
	gcc -c gfx3.c

final.o: final.c
	gcc -c final.c

finalfn.0: finalfn.c
	gcc -c finalfn.c

clean:
	rm *.o chiellini
