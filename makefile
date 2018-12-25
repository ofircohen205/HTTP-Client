# ©Dudi Hakakyan

all:
	-mkdir testResaults 
	-mkdir testResaults/test1 
	-mkdir testResaults/test2
	-mkdir testResaults/test3
	-mkdir testResaults/test4
	-mkdir testResaults/test5
	-mkdir testResaults/test6
	-mkdir testResaults/test7
	-mkdir testResaults/test8
	-mkdir testResaults/valgrind
	@echo '********************* COMPILING $@ *****************'	
	gcc -Wall client.c -o client 2> compilationWornings.txt														
	@echo '********** TEST 1: CHECKS PROTOCOL: ALL NEED TO WORK ************'
	-./client HTTP://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test1/t1-1.txt 2> testResaults/test1/t1-1-ERROR.txt						
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test1/t1-2.txt 2> testResaults/test1/t1-2-ERROR.txt	
	@echo '**************** TEST 2: ******************'
	-./client http:/// > testResaults/test2/t2-1.txt 2> testResaults/test2/t2-1-ERROR.txt						
	-./client http://:/ > testResaults/test2/t2-2.txt 2> testResaults/test2/t2-2-ERROR.txt
	-./client http// > testResaults/test2/t2-3.txt 2> testResaults/test2/t2-3-ERROR.txt						
	-./client http/:/ > testResaults/test2/t2-4.txt 2> testResaults/test2/t2-4-ERROR.txt
	-./client http//: > testResaults/test2/t2-5.txt 2> testResaults/test2/t2-5-ERROR.txt						
	-./client http:// > testResaults/test2/t2-6.txt 2> testResaults/test2/t2-6-ERROR.txt
	-./client httpa:// > testResaults/test2/t2-7.txt 2> testResaults/test2/t2-7-ERROR.txt
	-./client ahttp:// > testResaults/test2/t2-8.txt 2> testResaults/test2/t2-8-ERROR.txt
	@echo '**************** TEST 3: CHECKS PORT ******************'			
	-./client http://www.ynet.co.il:/home/0,7340,L-8,00.html > testResaults/test3/t3-1.txt 2> testResaults/test3/t3-1-ERROR.txt						
	-./client http://www.ynet.co.il:6s3 > testResaults/test3/t3-2.txt 2> testResaults/test3/t3-2-ERROR.txt 
	-./client http://www.ynet.co.il:-78 > testResaults/test3/t3-3.txt 2> testResaults/test3/t3-3-ERROR.txt						
	-./client http://www.ynet.co.il:kl > testResaults/test3/t3-4.txt 2> testResaults/test3/t3-4-ERROR.txt
	-./client http://www.ynet.co.il:80/home/0,7340,L-8,00.html > testResaults/test3/t3-5.txt 2> testResaults/test3/t3-5-ERROR.txt
	-./client http://www.ynet.co.il:980/home/0,7340,L-8,00.html > testResaults/test3/t3-6.txt 2> testResaults/test3/t3-6-ERROR.txt
	@echo '**************** TEST 4: CHECKS PATH ******************'	
	-./client http://www.ynet.co.il > testResaults/test4/t4-1.txt 2> testResaults/test4/t4-1-ERROR.txt					
	-./client http://www.ynet.co.il/ > testResaults/test4/t4-2.txt	2> testResaults/test4/t4-2-ERROR.txt	
	-./client http://www.ynet.co.il/: > testResaults/test4/t4-3.txt 2> testResaults/test4/t4-3-ERROR.txt							
	-./client http://www.ynet.co.il/:234 > testResaults/test4/t4-4.txt 2> testResaults/test4/t4-4-ERROR.txt	
	@echo '**************** TEST 5: WRONG COMMAND ***************'	
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -h 10:3:4 -d > testResaults/test5/t5-1.txt 2> testResaults/test5/t5-1-ERROR.txt
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -h -h > testResaults/test5/t5-2.txt 2> testResaults/test5/t5-2-ERROR.txt 
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d -d > testResaults/test5/t5-3.txt 2> testResaults/test5/t5-3-ERROR.txt
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d -h > testResaults/test5/t5-4.txt 2> testResaults/test5/t5-4-ERROR.txt 
	-./client -d http://www.ynet.co.il/home/0,7340,L-8,00.html -h > testResaults/test5/t5-5.txt 2> testResaults/test5/t5-5-ERROR.txt 
	-./client -h http://www.ynet.co.il/home/0,7340,L-8,00.html -h > testResaults/test5/t5-6.txt 2> testResaults/test5/t5-6-ERROR.txt 
	-./client -h -h http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test5/t5-7.txt 2> testResaults/test5/t5-7-ERROR.txt  
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -h -d asdk aksd jkn > testResaults/test5/t5-8.txt 2> testResaults/test5/t5-8-ERROR.txt
	-./client > testResaults/test5/t5-9.txt 2> testResaults/test5/t5-9-ERROR.txt
	-./client -d > testResaults/test5/t5-10.txt 2> testResaults/test5/t5-10-ERROR.txt
	-./client -d -h 0:0:4 http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test5/t5-11.txt 2> testResaults/test5/t5-11-ERROR.txt
	-./client -h 0:0:4 -d http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test5/t5-12.txt 2> testResaults/test5/t5-12-ERROR.txt
	-./client -d http://www.ynet.co.il/home/0,7340,L-8,00.html -h 0:0:4 > testResaults/test5/t5-13.txt 2> testResaults/test5/t5-13-ERROR.txt
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -h -r > testResaults/test5/t5-14.txt 2> testResaults/test5/t5-14-ERROR.txt
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -3 > testResaults/test5/t5-15.txt 2> testResaults/test5/t5-15-ERROR.txt 
	-./client -h asd http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test5/t5-16.txt 2> testResaults/test5/t5-16-ERROR.txt
	-./client -h -d http://www.ynet.co.il/home/0,7340,L-8,00.html 10:9:8 > testResaults/test5/t5-17.txt 2> testResaults/test5/t5-17-ERROR.txt
	@echo '***************** TEST 6: WRONG INPUT ****************'	
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d 10:3sd:4 > testResaults/test6/t6-1.txt 2> testResaults/test6/t6-1-ERROR.txt  
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d 10:3:5d3 -h > testResaults/test6/t6-2.txt 2> testResaults/test6/t6-2-ERROR.txt 
	-./client htp://www.ynet.co.il/home/0,7340,L-8,00.html -h > testResaults/test6/t6-3.txt 2> testResaults/test6/t6-3-ERROR.txt 
	-./client hhttp://www.ynet.co.il/home/0,7340,L-8,00.html -h > testResaults/test6/t6-4.txt 2> testResaults/test6/t6-4-ERROR.txt
	-./client www.ynet.co.il/home/0,7340,L-8,00.html -h > testResaults/test6/t6-5.txt 2> testResaults/test6/t6-5-ERROR.txt
	-./client -h -d http://www.ynet.co.il:95/home/0,7340,L-8,00.html s:9:d > testResaults/test6/t6-6.txt 2> testResaults/test6/t6-6-ERROR.txt
	-./client -h -d http://www.ynet.co.il:-8/home/0,7340,L-8,00.html s:9:d > testResaults/test6/t6-7.txt 2> testResaults/test6/t6-7-ERROR.txt
	-./client -h -d http://www.ynet.co.il:65536/home/0,7340,L-8,00.html s:9:d > testResaults/test6/t6-8.txt 2> testResaults/test6/t6-8-ERROR.txt        
	@echo '**************** TEST 7: HEAD REQUEST ******************'	
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -h > testResaults/test7/t7-1.txt 2> testResaults/test7/t7-1-ERROR.txt 				
	-./client -h http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test7/t7-2.txt 2> testResaults/test7/t7-2-ERROR.txt 
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -h -d -3:9:8 > testResaults/test7/t7-3.txt 2> testResaults/test7/t7-3-ERROR.txt
	-./client -h http://www.ynet.co.il/home/0,7340,L-8,00.html -d 4:2:1 > testResaults/test7/t7-4.txt 2> testResaults/test7/t7-4-ERROR.txt 
	-./client -d 3:4:5 http://www.ynet.co.il/home/0,7340,L-8,00.html -h > testResaults/test7/t7-5.txt 2> testResaults/test7/t7-5-ERROR.txt
	-./client -d 3:4:5 -h http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test7/t7-6.txt 2> testResaults/test7/t7-6-ERROR.txt 
	-./client -h -d 3:4:5 http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test7/t7-7.txt 2> testResaults/test7/t7-7-ERROR.txt 
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d 3:4:5 -h > testResaults/test7/t7-8.txt 2> testResaults/test7/t7-8-ERROR.txt
	@echo '**************** TEST 8: GET REQUEST ******************'	
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test8/t8-1.txt 2> testResaults/test8/t8-1-ERROR.txt 				
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test8/t8-2.txt 2> testResaults/test8/t8-2-ERROR.txt
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d -3:9:8 > testResaults/test8/t8-3.txt 2> testResaults/test8/t8-3-ERROR.txt
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d 4:-2:1 > testResaults/test8/t8-4.txt 2> testResaults/test8/t8-4-ERROR.txt
	-./client -d -3:-4:-5 http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test8/t8-5.txt 2> testResaults/test8/t8-5-ERROR.txt
	-./client -d 3:4:5 http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test8/t8-6.txt 2> testResaults/test8/t8-6-ERROR.txt 
	-./client -d 3:4:-5 http://www.ynet.co.il/home/0,7340,L-8,00.html > testResaults/test8/t8-7.txt 2> testResaults/test8/t8-7-ERROR.txt
	@echo '**************** VALGRIND test ******************'
	-valgrind --log-file="testResaults/valgrind/valgrind1.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -h
	-valgrind --log-file="testResaults/valgrind/valgrind2.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d 3:4:5
	-valgrind --log-file="testResaults/valgrind/valgrind3.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d sdf
	-valgrind --log-file="testResaults/valgrind/valgrind4.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -h -d sdad
	-valgrind --log-file="testResaults/valgrind/valgrind5.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d 43:5:1
	-valgrind --log-file="testResaults/valgrind/valgrind6.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d -4:-8:-9
	-valgrind --log-file="testResaults/valgrind/valgrind7.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html
	-valgrind --log-file="testResaults/valgrind/valgrind8.txt" ./client http://www.ynet.c
	-valgrind --log-file="testResaults/valgrind/valgrind9.txt" ./client www.ynet.co.il/home/0,7340,L-8,00.html
	-valgrind --log-file="testResaults/valgrind/valgrind10.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html
	-valgrind --log-file="testResaults/valgrind/valgrind11.txt" ./client http:///www.ynet.co.il/home/0,7340,L-8,00.html
	-valgrind --log-file="testResaults/valgrind/valgrind12.txt" ./client http://www.ynet.co.il:/home/0,7340,L-8,00.html
	-valgrind --log-file="testResaults/valgrind/valgrind13.txt" ./client http://www.ynet.co.il:78/home/0,7340,L-8,00.html
	@echo '******************** $@ FINISHED ****************'

.PHONY: all $(SUBDIRS)

