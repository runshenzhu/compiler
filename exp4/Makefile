Compiler: lex.l syntax.y
	bison -d syntax.y
	flex lex.l
	gcc -o compiler syntax.tab.c main.c Fun.c hash.c -lfl -ly
	#gcc syntax.tab.c main.c Fun.c hash.c -lfl -ly -fPIC -shared -o lib.so
clean:
	rm -f compiler *.yy.c *.tab.h *.tab.c
