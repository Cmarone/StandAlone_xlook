//This is a stand alone version of xlook. It doesn't have graphics. It's meant to run "_r file" scripts and produce ASCII output for plotting.
// 20240302 cjm.


/*
_r files were called by the 'doit' command in xlook. 
doit files can be nested
The pathnames for nesting is set up in cmds.c

doit files work by a procedure called in doit_proc() in cmds1.c
doit_proc reads lines, does some error checking and sends the command to command_handler() in event.c

recall that all of the commands are in command_handler in event.c
After the command_handler there is process_action(arg). This was the way things were processed from the xlook command line in X and GTK. It's a big switch(ui_globals.action) with all the commands. It does nothing for doit files...

todo
done. messages.c ->print_msg: make this come to std out
look at ui_globals.action and set MAIN to simply run the next line of a doit file..
look at process_action(input)... called by command_handler

*/

#include        <stdio.h>
#include        <stdlib.h>
#include        <math.h>
#define         SR_2   1.41421356
#define         TRUE 1
#define         FALSE 0

#include "array.h"
#include "config.h"
#include "cmds.h"
#include "cmds1.h"
#include "event.h"
#include "filtersm.h"
#include "fq.h"
#include "func.h"
#include "global.h"
#include "lookio.h"
#include "look_funcs.h"
#include "messages.h"
#include "mem.h"
#include "notices.h"
#include "nrutil.h"
#include "polyfit.h"
#include "qi_look.h"
//#include "simplexl.h"
#include "special.h"
#include "strcmd.h"


char    *progname;
char msg[MSG_LENGTH];
char data_file[20], new_file[20];
FILE *data, *fopen(), *new;
int doit_des, doit_f_open;
char pathname[10][1024], default_path[1024], metapath[1024];
int name_action;
int doit;

int main (int ac, char *av[])
{

char    outfile[64], string[65536], string1[65536], string2[65536];
/*char  *strcpy(), *index(), *strcat();*/
char    tim_out ='n', vel_out ='n', state_out ='n', porosity_time_out = 'n', sd_out = 'n';
char    segall_rice_law = 'n', doing_hold = 'n', doing_ramp = 'n';
char    set_ref_fric = 'n', do_mvs = 'n';
FILE    *r_file, *fopen();
int     ndata,i,j,kk,nsteps, vstep, data_count;
double  log(), exp();
double  ceil();
double  max_x, op_inc_disp, op_inc_hold;
double  *v_slider,*state,*sd, *phi, hold_time=0;
double  calc_time, temp_time;
double  temp, temp1, temp2, step_size, step_duration, lpDisp;


      progname = av[0];

        if(ac < 2)
        {
        fprintf(stderr,"\t\n This is a stand alone version of xlook. It doesn't have graphics. It's meant to run '_r file scripts and produce ASCII output for plotting. \n" );
        fprintf(stderr, "Usage is: %s filename, where filename is an xlook 'doit file' script\n You can find an example of a script and other xlook details on github here \n", progname);
	    exit(2);
        }

	if( (r_file = fopen( av[1], "r")) == NULL)
	{
                fprintf(stderr, "Can't open file %s\n",av[1]);
		exit(3);
	}
	else
                fprintf(stderr, "running doit file %s\n",av[1]);
	
	doit_proc(av[1]);
	
	exit(0);
}


