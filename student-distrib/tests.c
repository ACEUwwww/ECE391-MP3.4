#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "RTC.h"
#include "paging.h"
#include "filesystem.h"
#include "process.h"

#define PASS 1
#define FAIL 0

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 * 
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0; i < 20; ++i){
		if ((idt[i].offset_15_00 == NULL) && 
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}
	if ((idt[0x21].offset_15_00 == NULL) && 
			(idt[0x20].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	if ((idt[0x28].offset_15_00 == NULL) && 
			(idt[0x28].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	if ((idt[0x80].offset_15_00 == NULL) && 
			(idt[0x80].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	return result;
}

/* exception Test - Example
 * 
 * Asserts that devide by zero will cause an error
 * Inputs: None
 * Outputs: on the screen
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
void exception_test(){
	int myone = 1;
	int myzero = 0;
	int myout;
	myout = myone/myzero;
}

/* rtc_test1()
 * Decription: print a number by the frequency I set
 * Inputs: None
 * Outputs: FAIL/PASS
 * Side Effects: None
 * Coverage: RTC interrupt at the 32 hz
 * Files: RTC.h/c
 */
int rtc_test1()
{
	int i;
	 //set freq to 2hz (default)
	 rtc_open();
	int freq_test = 32;
	printf("The present frequency is %dhz.\n", freq_test);
	rtc_write(freq_test);
	for(i = 0; i < 1025; ++i) // 32, 10 just for test, can be modified when demo
	{
		rtc_read(issue.rtc_freq); // wait until RTC interrupt
		printf("%d ", i);
	}
	rtc_close();
	return 0;
}

/* rtc_test2()
 * 
 * Inputs: None
 * Outputs: FAIL/PASS
 * Side Effects: None
 * Coverage: RTC interrupt at the original 2 hz by the rtc_open
 * Files: RTC.h/c
 */
int rtc_test2()
{
	int i;
	 //set freq to 2hz (default)
	printf("The present frequency is %dhz.\n", 2);
	rtc_open();
	for(i = 0; i < 1025; ++i) // 32, 10 just for test, can be modified when demo
	{
		rtc_read(issue.rtc_freq); // wait until RTC interrupt
		printf("1 ");
	}
	rtc_close();
	return 0;
}



/* page_test_present - Example
 * 
 * check the initialize of PT\DT and each PTE\PDEs
 * Inputs: None
 * Outputs: FAIL/PASS
 * Side Effects: None
 * Coverage: 
 * Files: paging.h/c
 */
int page_test_present(void)
{
	int i;
	int result = PASS;
	if (((DT[0] & PDE_P)  == 0 )|| ((DT[1] & PDE_P) == 0 ))
	{
		printf ("The present value for the first two PDE is false.\n");
		result = FAIL;
	}
	for (i=0;i<1024;i++)
	{
		if (((DT[i] & PDE_P) == 1) && (i>1))
		{
			printf("Wrong present number of PD present for memory > 8MB\n");
			result = FAIL;
		}
		if (((PT[i] & PTE_P) != 1) && (i == (VIDEO_MEM >> 12)))
		{
			printf("Wrong present number at video memory PTE\n");
			result = FAIL;
		}
		if (((PT[i] & PTE_P) == 1) && (i != (VIDEO_MEM >> 12)))
		{
			printf("%d",i);
			printf("Wrong present number for PTE out of video memory\n");
			result = FAIL;
		}
	}

	printf("The test result is %d",result);
	return result;
}


/* page_test_margin - Example
 * 
 * check the start and end of video memory and kernel if they can works
 * Inputs: None
 * Outputs: FAIL/PASS
 * Side Effects: None
 * Coverage: page fault exception
 * Files: paging.h/c
 */
int page_test_margin()
{
	uint8_t * ptr ;
	int a;
	ptr = (uint8_t*)VIDEO_MEM;
	ptr[0] = 1;
	a = ptr[0];
	ptr = (uint8_t*)VIDEO_END ;	
	ptr[0] = 1;
	a = ptr[0];
	ptr = (uint8_t*)KERNEL_OFFSET ;
	ptr[0] = 1;
	a = ptr[0];
	ptr = (uint8_t*)KERNEL_END ;
	printf("The test reuslt is PASS\n");
	return PASS;
}

/* page_fault_test1 - Example
 * 
 * test start of vedio memory -1 's VM if works
 * Inputs: None
 * Outputs: FAIL/PASS
 * Side Effects: None
 * Coverage: page fault exception
 * Files: paging.h/c
 */
int page_fault_test1()
{
	uint8_t * ptr ;
	int a;
	ptr = (uint8_t*)(VIDEO_MEM - 1) ;
	a=ptr[0];
	printf("The test reuslt is FAIL\n");
	return FAIL;
}

/* page_fault_test2 - Example
 * 
 * test end of vedio memory +1 's VM if works
 * Inputs: None
 * Outputs: FAIL/PASS
 * Side Effects: None
 * Coverage: page fault exception
 * Files: paging.h/c
 */
int page_fault_test2()
{
	uint8_t * ptr ;
	int a;
	ptr = (uint8_t*)(VIDEO_END +1 ) ;
	a=ptr[0];
	printf("The test reuslt is PASS\n");
	return FAIL;
}

/* page_fault_test3 - Example
 * 
 * test start of kernel -1 's VM if works
 * Inputs: None
 * Outputs: FAIL/PASS
 * Side Effects: None
 * Coverage: page fault exception
 * Files: paging.h/c
 */
int page_fault_test3()
{
	uint8_t * ptr ;
	int a;
	ptr = (uint8_t*)(KERNEL_OFFSET - 1) ;
	a=ptr[0];
	printf("The test reuslt is PASS\n");
	return FAIL ;
}

/* page_fault_test4 - Example
 * 
 * test end of kernel +1 's VM if works
 * Inputs: None
 * Outputs: FAIL/PASS
 * Side Effects: None
 * Coverage: page fault exception	
 * Files: paging.h/c
 */
int page_fault_test4()
{
	uint8_t * ptr ;
	int a;
	ptr = (uint8_t*)(KERNEL_END + 1) ;
	a=ptr[0];
	printf("The test reuslt is PASS\n");
	return FAIL;
}


// add more tests here
/* Checkpoint 2 tests */
/* open file test
	Input: nan
	Output: SUCCESS or FAILURE to the screen
	return value: nan
*/

// void read_file_test(int filechosen)
// {	
// 	clean_screen();
// 	if (filechosen < 0 || filechosen >=17) {printf("INVALID INPUT FOR THIS TESTING FUNCTION"); return;}
// 	read_test_help(filechosen);
// }

/* Checkpoint 3 tests */
// void systemrw(const char *fname)
// {
// 	clean_screen();
// 	char *buf;
// 	int i;
// 	int retval;
// 	init_fdarray();
// 	open(fname);
// }	

/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	// idt_test();
	// rtc_test1(); //print a number by the frequency I set, which is 32hz here
	// rtc_test2(); //print a number by the frequency set by rtc_open, which is 2hz here
	//page_test_present();
	//page_test_margin();
	//page_fault_test1();
	//page_fault_test2();
	//page_fault_test3();
	//page_fault_test4();
	// launch your tests here
     // read_file_test(JDG 2022 World Champion);
	// list_all_file();
	// systemrw("frame0.txt");
	return;
}



