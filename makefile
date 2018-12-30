# ©Ofir Cohen

all:
	@echo '****************************************************'
	@echo '********************* START TESTER *****************'	
	@echo '****************************************************'


	-mkdir testResults 
	-mkdir testResults/test1 
	-mkdir testResults/test2
	-mkdir testResults/test3
	-mkdir testResults/test4
	-mkdir testResults/test5
	-mkdir testResults/test6
	-mkdir testResults/test7
	-mkdir testResults/test8
	#-mkdir testResults/valgrind
	
	@echo '********************* COMPILING $@ *****************'	
	gcc client.c -o client -Wall 2> compilationWarnings.txt														
	

	@echo ''
	@echo '********** TEST 1: CHECKS PROTOCOL: ALL NEED TO WORK ************'
	-./client http://www.ynet.co.il/home/0,7340,L-8,00.html > testResults/test1/t1-1.txt 2> testResults/test1/t1-1-ERROR.txt						
	-./client http://www.ptsv2.com/t/ex2_tester/post > testResults/test1/t1-2.txt 2> testResults/test1/t1-2-ERROR.txt	
	

	# @echo ''
	# @echo '**************** TEST 2: CHECKS HTTP ******************'
	# -./client http:/// > testResults/test2/t2-1.txt 2> testResults/test2/t2-1-ERROR.txt						
	# -./client http://:/ > testResults/test2/t2-2.txt 2> testResults/test2/t2-2-ERROR.txt
	# -./client http// > testResults/test2/t2-3.txt 2> testResults/test2/t2-3-ERROR.txt						
	# -./client http/:/ > testResults/test2/t2-4.txt 2> testResults/test2/t2-4-ERROR.txt
	# -./client http//: > testResults/test2/t2-5.txt 2> testResults/test2/t2-5-ERROR.txt						
	# -./client http:// > testResults/test2/t2-6.txt 2> testResults/test2/t2-6-ERROR.txt
	# -./client httpa:// > testResults/test2/t2-7.txt 2> testResults/test2/t2-7-ERROR.txt
	# -./client ahttp:// > testResults/test2/t2-8.txt 2> testResults/test2/t2-8-ERROR.txt
	

	@echo ''
	@echo '**************** TEST 3: CHECKS PORT ******************'			
	-./client http://www.ptsv2.com:/t/ex2_tester/post > testResults/test3/t3-1.txt 2> testResults/test3/t3-1-ERROR.txt						
	-./client http://www.ynet.co.il:6s3 > testResults/test3/t3-2.txt 2> testResults/test3/t3-2-ERROR.txt 
	-./client http://www.ynet.co.il:-78 > testResults/test3/t3-3.txt 2> testResults/test3/t3-3-ERROR.txt						
	-./client http://www.ynet.co.il:kl > testResults/test3/t3-4.txt 2> testResults/test3/t3-4-ERROR.txt
	-./client http://www.ynet.co.il:80/home/0,7340,L-8,00.html > testResults/test3/t3-5.txt 2> testResults/test3/t3-5-ERROR.txt
	# -./client http://www.ynet.co.il:980/home/0,7340,L-8,00.html > testResults/test3/t3-6.txt 2> testResults/test3/t3-6-ERROR.txt
	

	@echo ''
	@echo '**************** TEST 4: CHECKS PATH ******************'	
	-./client http://www.ptsv2.com > testResults/test4/t4-1.txt 2> testResults/test4/t4-1-ERROR.txt					
	-./client http://www.ptsv2.com/ > testResults/test4/t4-2.txt	2> testResults/test4/t4-2-ERROR.txt	
	-./client http://www.ptsv2.com/t/ex2_tester: > testResults/test4/t4-3.txt 2> testResults/test4/t4-3-ERROR.txt							
	-./client http://www.ptsv2.com/t/ex2_tester:234 > testResults/test4/t4-4.txt 2> testResults/test4/t4-4-ERROR.txt	
	

	@echo ''
	@echo '**************** TEST 5: WRONG COMMAND ***************'	
	-./client http://www.ptsv2.com/t/ex2_tester -p text -r > testResults/test5/t5-1.txt 2> testResults/test5/t5-1-ERROR.txt
	-./client http://www.ptsv2.com/t/ex2_tester -r -p > testResults/test5/t5-2.txt 2> testResults/test5/t5-2-ERROR.txt 
	-./client http://www.ptsv2.com/t/ex2_tester -r > testResults/test5/t5-3.txt 2> testResults/test5/t5-3-ERROR.txt
	-./client http://www.ptsv2.com/t/ex2_tester -p > testResults/test5/t5-4.txt 2> testResults/test5/t5-4-ERROR.txt 
	-./client -p http://www.ptsv2.com/t/ex2_tester -r 1 bla1=bla1 bla2=bla2 > testResults/test5/t5-5.txt 2> testResults/test5/t5-5-ERROR.txt 
	-./client -r http://www.ptsv2.com/t/ex2_tester -h > testResults/test5/t5-6.txt 2> testResults/test5/t5-6-ERROR.txt 
	-./client -r 1 -p http://www.ptsv2.com/t/ex2_tester > testResults/test5/t5-7.txt 2> testResults/test5/t5-7-ERROR.txt  	
	-./client http://www.ptsv2.com/t/ex2_tester -r 1 10:3sd:4 > testResults/test6/t6-1.txt 2> testResults/test6/t6-1-ERROR.txt  
	-./client http://www.ptsv2.com/t/ex2_tester -r 5 10:3:5d3 -p > testResults/test6/t6-2.txt 2> testResults/test6/t6-2-ERROR.txt 
	-./client ttp://www.ptsv2.com/t/ex2_tester -p > testResults/test6/t6-3.txt 2> testResults/test6/t6-3-ERROR.txt 
	-./client htttp://www.ptsv2.com/t/ex2_tester -r 1 > testResults/test6/t6-4.txt 2> testResults/test6/t6-4-ERROR.txt
	-./client http://www.ptsv2.com/t/ex2_tester -p > testResults/test6/t6-5.txt 2> testResults/test6/t6-5-ERROR.txt
	-./client -p -r http://www.ptsv2.com/t/ex2_tester -r 1 s:9:d > testResults/test6/t6-6.txt 2> testResults/test6/t6-6-ERROR.txt
	-./client -p -r 1 bla=bla http://www.ptsv2.com/t/ex2_tester > testResults/test6/t6-7.txt 2> testResults/test6/t6-7-ERROR.txt
	-./client -r 0 hi=hi http://www.ptsv2.com/t/ex2_tester > testResults/test6/t6-8.txt 2> testResults/test6/t6-8-ERROR.txt        
	

	@echo ''
	@echo '**************** TEST 7: POST REQUEST ******************'	
	-./client http://www.ptsv2.com/t/ex2_tester -p text > testResults/test7/t7-1.txt 2> testResults/test7/t7-1-ERROR.txt 				
	-./client -p shaharHaTahat http://www.ptsv2.com/t/ex2_tester > testResults/test7/t7-2.txt 2> testResults/test7/t7-2-ERROR.txt 
	-./client http://www.ptsv2.com/t/ex2_tester -p hello -r 1 bla=bla > testResults/test7/t7-3.txt 2> testResults/test7/t7-3-ERROR.txt
	-./client -p blabla http://www.ptsv2.com/t/ex2_tester -r 2 bla=bla bla1=bla1 > testResults/test7/t7-4.txt 2> testResults/test7/t7-4-ERROR.txt 
	-./client -p hello http://www.ptsv2.com/t/ex2_tester -r 2 val=val1 val2=val > testResults/test7/t7-5.txt 2> testResults/test7/t7-5-ERROR.txt
	-./client -r 1 hello=world -p helloWorld http://www.ptsv2.com/t/ex2_tester > testResults/test7/t7-6.txt 2> testResults/test7/t7-6-ERROR.txt 

	

	@echo ''
	@echo '**************** TEST 8: GET REQUEST ******************'	
	-./client http://www.ptsv2.com/t/ex2_tester > testResults/test8/t8-1.txt 2> testResults/test8/t8-1-ERROR.txt 				
	-./client http://www.ptsv2.com/t/ex2_tester > testResults/test8/t8-2.txt 2> testResults/test8/t8-2-ERROR.txt
	-./client http://www.ptsv2.com/t/ex2_tester -r 1 val=val1 > testResults/test8/t8-3.txt 2> testResults/test8/t8-3-ERROR.txt
	-./client http://www.ptsv2.com/t/ex2_tester -r 3 how=are you=im fine=thanks > testResults/test8/t8-4.txt 2> testResults/test8/t8-4-ERROR.txt
	

	# @echo ''
	# @echo '**************** VALGRIND test ******************'
	# -valgrind --log-file="testResults/valgrind/valgrind1.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -h
	# -valgrind --log-file="testResults/valgrind/valgrind2.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d 3:4:5
	# -valgrind --log-file="testResults/valgrind/valgrind3.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d sdf
	# -valgrind --log-file="testResults/valgrind/valgrind4.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -h -d sdad
	# -valgrind --log-file="testResults/valgrind/valgrind5.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d 43:5:1
	# -valgrind --log-file="testResults/valgrind/valgrind6.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html -d -4:-8:-9
	# -valgrind --log-file="testResults/valgrind/valgrind7.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html
	# -valgrind --log-file="testResults/valgrind/valgrind8.txt" ./client http://www.ynet.c
	# -valgrind --log-file="testResults/valgrind/valgrind9.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html
	# -valgrind --log-file="testResults/valgrind/valgrind10.txt" ./client http://www.ynet.co.il/home/0,7340,L-8,00.html
	# -valgrind --log-file="testResults/valgrind/valgrind11.txt" ./client http:///www.ynet.co.il/home/0,7340,L-8,00.html
	# -valgrind --log-file="testResults/valgrind/valgrind12.txt" ./client http://www.ynet.co.il:/home/0,7340,L-8,00.html
	# -valgrind --log-file="testResults/valgrind/valgrind13.txt" ./client http://www.ynet.co.il:78/home/0,7340,L-8,00.html
	

	@echo ''
	@echo '****************************************************'
	@echo '********************* END TESTER *****************'	
	@echo '****************************************************'


.PHONY: all $(SUBDIRS)

