/* File operation table. Created by Group44 YG*/
/* get fot function
    Description: set up relative entries in file operation table
    Input: nan
    Output: entry
    Return value: related entry
*/
#include "fileoptable.h"
#include "filesystem.h"
#include "terminal.h"
#include "RTC.h"

fileot get_terminal_fot(void)
{
    fileot terminal_fot;
    terminal_fot.open = terminal_open;
    terminal_fot.close = terminal_close;
    terminal_fot.read = terminal_read;
    terminal_fot.write = terminal_write;
    return terminal_fot;
}
/* get fot function
    Description: set up relative entries in file operation table
    Input: nan
    Output: entry
    Return value: related entry
*/
fileot get_file_fot(void){
    fileot file_fot;
    file_fot.open = file_open;
    file_fot.close = file_close;
    file_fot.read = file_read;
    file_fot.write = file_write;
    return file_fot;
}
/* get fot function
    Description: set up relative entries in file operation table
    Input: nan
    Output: entry
    Return value: related entry
*/
fileot get_dir_fot(void){
    fileot dir_fot;
    dir_fot.open = directory_open;
    dir_fot.close = directory_close;
    dir_fot.read = directory_read;
    dir_fot.write = directory_write;
    return dir_fot;
}

fileot get_rtc_fot(void){
    fileot rtc_fot;
    rtc_fot.open = rtcopen;
    rtc_fot.close = rtcclose;
    rtc_fot.read = rtcread;
    rtc_fot.write = rtcwrite;
    return rtc_fot;
}

/* init_fot
    Description: set up the file operation table
    Input: nan
    Output: file operation table set up
    Return value: nan
*/
void init_fot(void){
    // file_operation_table[rtc_in_fot] = get_rtc_fot();
    file_operation_table[rtc_in_fot] = get_rtc_fot();
    file_operation_table[dir_in_fot] = get_dir_fot();
    file_operation_table[file_in_fot] = get_file_fot();
    file_operation_table[ter_in_fot] = get_terminal_fot();
}

/* Open file. 1 on success, -1 on failure*/

int rtcopen(const char* fname)
{
    rtc_open();
    return 0;
}

/* file_close
    Description: close the files
    Input: nan
    Output: nan
    Return value: always success. Nothing to do.
*/
int rtcclose(const char* fname)
{
    rtc_close();
    return 0;
}
/* file_read
    Description: read files
    Input: same as read_data
    Output: same as read_data
    Return value: same as read_data
*/
int rtcread(const char* fname, uint32_t offset, void* buf, int32_t length)
{
    rtc_read(2);
    return 0;
}

/* file_write
    Description: write to files
    Input: nan
    Output: nan
    Return value: always failure. Can't write to read-only files
*/
int rtcwrite(int32_t fd, const void* buf, int32_t nbytes)
{
    rtc_write(2);
    return 0;
}
