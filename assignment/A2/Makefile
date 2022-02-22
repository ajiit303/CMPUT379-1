target=		submit
allFiles=       Makefile handler.cpp main.cpp master.cpp message.cpp rule.cpp tor.cpp utils.cpp const.h handler.h master.h message.h rule.h tor.h utils.h
scpDir=		chengxua@ug05.cs.ualberta.ca:~/379
# ------------------------------------------------------------


starter:  handler.cpp main.cpp master.cpp message.cpp rule.cpp tor.cpp utils.cpp const.h handler.h master.h message.h rule.h tor.h utils.h
	g++ -Wall handler.cpp main.cpp master.cpp rule.cpp tor.cpp utils.cpp -o a2w22

tar:
	touch $(target).tar.gz
	mv $(target).tar.gz  x$(target).tar.gz
	tar -cvf $(target).tar $(allFiles)
	gzip $(target).tar

scp:
	scp $(target).tar.gz $(scpDir)

clean:
	find ./ -name "fifo*" -delete
