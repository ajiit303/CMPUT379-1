target=		submit
allFiles=       Makefile handler.cpp main.cpp master.cpp message.cpp rule.cpp tor.cpp utils.cpp const.h handler.h master.h message.h rule.h tor.h utils.h
scpDir=		chengxua@ug05.cs.ualberta.ca:~/379
# ------------------------------------------------------------


starter:  handler.cpp main.cpp master.cpp message.cpp rule.cpp tor.cpp utils.cpp const.h handler.h master.h message.h rule.h tor.h utils.h
	g++ -Wall -pthread handler.cpp main.cpp master.cpp message.cpp rule.cpp tor.cpp utils.cpp -o a2w22

tar:
	touch $(target).tar.gz
	mv $(target).tar.gz  x$(target).tar.gz
	tar -cvf $(target).tar $(allFiles)
	gzip $(target).tar

s1:
	./a2w22 psw1 file3.dat null psw2 100-110

s2:
	./a2w22 psw2 file3.dat psw1 null 200-210

master:
	./a2w22 master 2

group:
	./a2w22 master 2
	./a2w22 psw1 file3.dat null psw2 100-110
	./a2w22 psw2 file3.dat psw1 null 200-210

scp:
	scp $(target).tar.gz $(scpDir)

clean:
	find ./ -name "fifo*" -delete
