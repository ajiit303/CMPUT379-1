target=		submit
allFiles=       Makefile main.cpp master.cpp rule.cpp tor.cpp utils.cpp const.h master.h rule.h tor.h utils.h
scpDir=		chengxua@ug05.cs.ualberta.ca:~/379
# ------------------------------------------------------------


starter:  main.cpp master.cpp rule.cpp tor.cpp utils.cpp const.h master.h rule.h tor.h utils.h
	g++ -Wall main.cpp master.cpp rule.cpp tor.cpp utils.cpp -o a2w22

tar:
	touch $(target).tar.gz
	mv $(target).tar.gz  x$(target).tar.gz
	tar -cvf $(target).tar $(allFiles)
	gzip $(target).tar

scp:
	scp $(target).tar.gz $(scpDir)

clean:
	rm *~ out.* *.o *.exe
