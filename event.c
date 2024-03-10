#include <string.h>
#include <sys/file.h>

#include "cmds.h"
#include "cmds1.h"
#include "config.h"
#include "event.h"
#include "global.h"
#include "look_funcs.h"
#include "messages.h"
#include "notices.h"
#include "simplexl.h"
//#include "simplexl.h"
#include "strcmd.h"

char tmp_cmd[1024];
/* cjm 14.5.07; to solve problem with doit files: increased the size of path names. 1024's used to be 80, 512 used to be 32 */
/*cjm 20150518: I commented out these lines (the next 6, plus the FILE *data, *new), b/c they seem to be re-definitions of global variables with the same names (see e.g., cmds1.c*/
//char pathname[10][1024];
//char default_path[1024];
//char metapath[1024];
//char data_file[512];
//char new_file[512];
//char headline[512];

char qiparams[1024];

//FILE *data, *new;

char plot_cmd[16], trig_cmd[16], qi_cmd[16], type_cmd[16];

extern char msg[MSG_LENGTH];
extern char    verbose, write_all_at_end;

int nargs;
char *args_left;

//extern char mu_fit_mess_1[512];


void command_handler(char input[]) 
     //char input[256]; 
{ 
  char cmd[256]; 
  char arg1[256], arg2[256], arg3[256], arg4[256]; 
  static char t_string[256];
  int nar, int1, i, intar[MAX_PLOTS], intar2[MAX_PLOTS];
  char *strtok();
   

  /*if (//ui_globals.action != MAIN)
    {
      process_action(input);
      return;
    }*/
  sscanf(input, "%s", cmd); 	/*take first part of input as cmd*/

  nargs = token_count(input,256);


    if ( (strcmp(cmd, "q") == 0) || (strncmp(cmd, "quit", 4) == 0) )
    {
      /* may need to free all the memory allocations */
	exit(0);
      //quit_xlook();
    }


  else if (strncmp(cmd, "set_path", 8) == 0) 
    {
      if (sscanf(input, "%s %s", arg1, arg2) == 2)   
	{
	  set_path_proc(arg2); 	 	
	  ////ui_globals.action = MAIN;
	} 
      else 	 	
	{ 	 
	  set_cmd_prompt("Pathname: ");
	  set_left_footer("Type the pathname");
	  //ui_globals.action = SET_PATH; 	
	}
    }

  
  else if (strncmp(cmd, "doit", 4) == 0)
    {
      if (sscanf(input, "%s %s", arg1, arg2) == 2) 	 	
	{
	  doit_proc(arg2); 	 	
	  ////ui_globals.action = MAIN;
	} 
      else 	 	
	{ 	 
	  set_cmd_prompt("Filename: ");
	  set_left_footer("Type the doit filename");
	  //ui_globals.action=DOIT; 	
	}
    }  
  

/*  else if (strncmp(cmd, "all", 3) == 0)
    {
      if (nargs == 4)
	{
	  all_final_proc(input);
	  ////ui_globals.action = MAIN;
	  set_left_footer("Type a command");
	  set_cmd_prompt("Command: ");
	}
      
      else
	//all_show_warning_proc(input);
    }
 */ 

  else if (strncmp(cmd, "read", 4) == 0 || strncmp(cmd, "append", 6) == 0)
    {
      if (nargs == 2)
	{
	  read_proc(input);
	  ////ui_globals.action = MAIN;
	}
      else
	{
	  if (strncmp(cmd, "read", 4) == 0)
	    {
	      set_left_footer("Type the data file to read");	  
	      //ui_globals.action = READ;
	    }
	  else 
	    {
	      set_left_footer("Type the file to append");
	      //ui_globals.action = APPEND;
	    }
	  set_cmd_prompt("Filename: ");
	  return;
	}
    }


  else if (strncmp(cmd, "write", 5) == 0)
  {
      if (sscanf(input, "%s %s", arg1, arg2) == 2)
	{
	  write_proc(arg2);
	  ////ui_globals.action = MAIN;
	}
      else
	{
	  set_left_footer("Type the filename to write");
	  set_cmd_prompt("Filename: ");
	  //ui_globals.action = WRITE;
	  
	}
  }

  /*  Tue Apr 15 21:25:02 EDT 1997 
      by park
      adding 'qi_tool' command */

  else if( (strncmp(cmd, "qi_tool", 7) == 0) || (strncmp(cmd, "fric_tool", 9) == 0)) 
  {
    strcpy(qiparams, input);
    if(nargs != 21)
    {
      sprintf(msg, "Error. 20 arguments are needed, only %d were entered. Relaunch tool or enter by hand\n", nargs-1);
      print_msg(msg); 
    }
    //qi_win_proc();
  }
  

  else if (strncmp(cmd, "examin", 6) == 0) 
    {
      if (sscanf(input, "%s %s", arg1, arg2) == 2) 	 	
	{
	  do_examin(arg2); 	 	
	  ////ui_globals.action = MAIN;
	} 
      else 	 	
	{ 	 
	  set_cmd_prompt("Filename: ");
	  set_left_footer("Type the input filename");	  //ui_globals.action = EXAMIN_GET_FILENAME; 	
	}
    }


  else if (strncmp(cmd, "getaschead", 10) == 0) 
    {
       if (sscanf(input, "%s %s", arg1, arg2) == 2) 	 	
	{
	  do_getaschead(arg2); 	 	
	  ////ui_globals.action = MAIN;
	} 
      else 	 	
	{ 	 
	  set_cmd_prompt("Filename: ");
	  set_left_footer("Type the input filename");
	  //ui_globals.action = GETASCHEAD_GET_FILENAME; 	
	}
    }


  else if (strncmp(cmd, "head", 4) == 0)
    {
       if (sscanf(input, "%s %s", arg1, arg2) == 2) 	 	
	{
	  do_head(arg2); 	 	
	  ////ui_globals.action = MAIN;
	} 
      else 
	{ 	 
	  set_cmd_prompt("Filename: ");
	  set_left_footer("Type the output filename or S for screen output.");
	  //ui_globals.action = HEAD_GET_FILENAME; 	
	}
     }
  
  else if (strncmp(cmd, "simplex", 7) == 0) 
    {
	fprintf(stderr,"simplex is no longer an option\n");

 /*     simplex_info();
      if (nargs == 6)
	{
	  do_simplex(input); 	 	
	} 
      else 	 	
	{ 	 
	  if (nargs > 1)
	    stripper(input, 1);

	  set_cmd_prompt("Function: ");
	  set_left_footer("Type the function to use");
	  //ui_globals.action = SIMPLEX_GET_FN; 	
	}
 */ 
    }

  else if (strncmp(cmd, "offset_int", 10) == 0) 
    {
      if (nargs == 5)
	{
	  do_offset_int(input); 	 	
	} 
      else 	 	
	{ 
	  sprintf(msg, "Offset COL (from REC2 to the end) by the difference between REC2 and REC1");
    	  if(verbose == 'y')
	  	print_msg(msg);
	  if (nargs > 1)
	    stripper(input, 1);

	  set_cmd_prompt("Col, Rec1, Rec2: ");
	  set_left_footer("Type the column (COL), record1 (REC1) and record2 (REC2)");
	  //ui_globals.action = OFFSET_INT; 	
	}
    }


  else if (strncmp(cmd, "offset", 6) == 0) 
    {
      if (nargs == 5)
	{
	  do_offset(input); 	 	
	  ////ui_globals.action = MAIN;
	} 
      else 	 	
	{ 
	  sprintf(msg, "Offset COL1 by the difference between COL2, REC2 and COL1, REC1\n");
    	  if(verbose == 'y')
	  	print_msg(msg);
	  if (nargs > 1)
	    stripper(input, 1);

	  set_cmd_prompt("Col1, Rec1, Col2, Rec2: ");
	  set_left_footer("Type COL1, REC1, COL2 and REC2");
	  //ui_globals.action = OFFSET; 	
	}
    }
 

  else if (strncmp(cmd, "zero", 4) == 0)
    { 
      if (nargs == 3)
	{
	  do_zero(input);
	  ////ui_globals.action = MAIN;
	} 
      else 	 	
	{ 	 
	  if (nargs > 1)
	    stripper(input, 1);

	  set_cmd_prompt("Col, Rec: ");
	  set_left_footer("Type the column and record number");
	  //ui_globals.action = ZERO; 	
	}
    }


 else if (strncmp(cmd, "r_row_col", 9) == 0 ||
	  strncmp(cmd, "rrc", 3) == 0)
    {
       if (nargs == 4)
	{
	  do_r_row_col(input);
	  ////ui_globals.action = MAIN;
	} 
      else 	 	
	{ 
	  if (nargs > 1)
	    stripper(input, 1);

	  set_cmd_prompt("Col: ");
	  set_left_footer("Which column to remove?");
	  //ui_globals.action = R_ROW_COL; 	
	}
    }


  else if (strncmp(cmd, "r_row", 5) == 0 || strncmp(cmd, "rr", 2) == 0) 
    {
       if (nargs == 3)
	{
	  do_r_row(input);
	  ////ui_globals.action = MAIN;
	} 
      else 	 	
	{ 	
	  if (nargs > 1)
	    stripper(input, 1);

	  set_cmd_prompt("First, Last: ");
	  set_left_footer("Type the first and last row to remove");
	  //ui_globals.action = R_ROW_F_L; 	
	}
    }


  else if (strncmp(cmd, "r_col", 5) == 0)
    { 
      if (sscanf(input, "%s %d", arg1, &int1) == 2)
	{
	  do_r_col(int1);
      	  ////ui_globals.action = MAIN;
	} 
      else 	 	
	{ 	
	  if (nargs > 1)
	    stripper(input, 1);

	  set_cmd_prompt("Col: ");
	  set_left_footer("Remove which column?");
	  //ui_globals.action = R_COL; 	
	}
    }


  else if (strncmp(cmd, "name", 4) == 0)
    { 
      if (nargs == 4)
	  do_name(input);
      else 	 	
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the column number to name, the name and the unit");
	  set_cmd_prompt("Col, Name, Unit: ");
	  //ui_globals.action = NAME; 	
	}
    }


  else if (strncmp(cmd, "comment", 7) == 0) 
    {
       if (nargs == 3)
	{
	  do_comment(input); 	 	
	  ////ui_globals.action = MAIN;
	} 
      else 	 	
	{ 
	  if (nargs > 1)
	    stripper(input, 1);

	  set_cmd_prompt("Col: ");
	  set_left_footer("Comment which column?");
	  //ui_globals.action = COMMENT_COL; 	
	}
    }


  else if (strncmp(cmd, "pdfauto", 7) == 0)
    {
      if (nargs == 5)
	{ 
	  do_pdfauto(input);
	  ////ui_globals.action = MAIN;
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);
	
	  set_left_footer("Type the Col, NewCol Prob, NewCol Bin, #Bins");
	  set_cmd_prompt("Col, NCP, NCB, #Bins: ");
	  //ui_globals.action = PDFAUTO;
	}
    }

  else if (strncmp(cmd, "pdf", 3) == 0)
    {
      if (nargs == 7)
	{ 
	  do_pdf(input);
	  ////ui_globals.action = MAIN;
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the Col, NewCol Prob, NewCol Bin, #Bins, Max, Min");
	  set_cmd_prompt("Col, NCP, NCB, #Bins, Max, Min: ");
	  //ui_globals.action = PDF;
	}
    }  
      
  else if (strncmp(cmd, "decimat", 7) == 0)
    {
      if (cmd[strlen((char *)cmd)-1] == 'h')
      {  
        sprintf(msg, "Decimate help.\nUsage: col, new_col, interval, first, last, name, units.\nRemove points from a segment of a column.\nInterval controls how often to save points. Higher intervals remove more points.\nUse decimate_r to remove rows after the last decimated row.\n\n");
	if(verbose == 'y')
        	print_msg(msg);
        //ui_globals.action=MAIN;
      }  
      else
      {   
         if (nargs == 8)
        {
          do_decimat(input);
        }
        else
        {
          if (nargs > 1)
            stripper(input, 1);

          set_left_footer("Type the Col, New Col and Increment");
          set_cmd_prompt("Col, New_Col, Inc: ");
 
          //if (strncmp(cmd, "decimat_r", 9) == 0)
            //ui_globals.action = DECIMAT_R;
          //else
            //ui_globals.action = DECIMAT;
        }
      }
    }
 
  else if (strncmp(cmd, "peak", 4) == 0)
    {
      if (nargs == 5)
	{ 
	  do_peak(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the Col and New Col");
	  set_cmd_prompt("Col, New_Col: ");
	  //ui_globals.action = PEAK;
	}
    }
  
  else if (strncmp(cmd, "curv", 4) == 0)
    {
      if (nargs == 6)
	{ 
	  do_curv(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the column number, new column number and dx");
	  set_cmd_prompt("Col, New_Col, dx: ");
	  //ui_globals.action = CURV;
	}
    }

  else if (strncmp(cmd, "summation", 9) == 0)
    {
      if (nargs == 5)
	{ 
	  do_summation(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the Col and New Col");
	  set_cmd_prompt("Col, New_Col: ");
	  //ui_globals.action = SUMMATION;
	}
    }

  else if (strncmp(cmd, "findint", 7) == 0)
    {
      if (nargs == 4)
	{ 
	  do_findint(input);
	  ////ui_globals.action = MAIN;
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the Column, First and Last record number");
	  set_cmd_prompt("Col, First, Last: ");
	  //ui_globals.action = FINDINT;
	}
    }

  else if (strncmp(cmd, "trend_a", 7) == 0)
    {
      if (nargs == 3)
	{ 
	  do_trend_a(input);
	  ////ui_globals.action = MAIN;
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

          sprintf(msg, "Calculate a best fit line.\n");
	  if(verbose == 'y')
      	  	print_msg(msg);
	  set_left_footer("Type X-column and Y-column");
	  set_cmd_prompt("X-col, Y-col: ");
	  //ui_globals.action = TREND_A;
	}
    }

  else if (strncmp(cmd, "trend", 5) == 0)
    {
      if (nargs == 5)
	{ 
	  do_trend(input);
	  ////ui_globals.action = MAIN;
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

          sprintf(msg, "Calculate a best fit line.\n");
	  if(verbose == 'y')
      	  	print_msg(msg);
	  set_left_footer("Type X-column and Y-column");
	  set_cmd_prompt("X-col, Y-col: ");
	  //ui_globals.action = TREND_XY;
	}
    }


  else if (strncmp(cmd, "r_trend", 7) == 0)
    {

      if (nargs == 10)
	{ 
	  strcpy(arg3,"\t ,");
	  strcpy(t_string,input);
	  strtok(t_string,arg3);	/*call twice so that we're pointing at 3rd tok*/
          strtok(NULL,arg3);
	  strcpy(arg4, strtok(NULL,arg3));
	  if(strncmp(arg4, "i", 1) == 0 || strncmp(arg4, "I", 1) == 0)
	  	do_r_trend_input(input);
	  else
	  	do_r_trend_comp(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

          sprintf(msg, "Remove a trend from data.\n");
	  if(verbose == 'y')
      	  	print_msg(msg);
	  set_left_footer("Detrend X or Y values?");
	  set_cmd_prompt("X/Y: ");
	  //ui_globals.action = R_TREND;
	}
    }

  else if (strncmp(cmd, "z_max", 5) == 0)
    {
      if (nargs == 5)
	{ 
	  do_z_max(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type Col and New Col");
	  set_cmd_prompt("Col, New_Col: ");
	  //ui_globals.action = Z_MAX;
	}
    }

  else if (strncmp(cmd, "z_min", 5) == 0)
    {
      if (nargs == 5)
	{ 
	  do_z_min(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type COL and NEW_COL");
	  set_cmd_prompt("Col, New_Col: ");
	  //ui_globals.action = Z_MIN;
	}
    }


  else if (strncmp(cmd, "r_mean", 6) == 0)
    {
      if (nargs == 5)
	{ 
	  do_r_mean(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type COL and NEW_COL");
	  set_cmd_prompt("Col, New_Col: ");
	  //ui_globals.action = R_MEAN;
	}
    }

  else if (strncmp(cmd, "compress", 8) == 0)
    {
      if (nargs == 5)
	{ 
	  do_compress(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type COL and NEW_COL");
	  set_cmd_prompt("Col, New_Col: ");
	  //ui_globals.action = COMPRESS;
	}
    }

  else if (strncmp(cmd, "positive", 8) == 0)
    {
      if (nargs == 5)
	{ 
	  do_positive(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type COL and NEW_COL");
	  set_cmd_prompt("Col, New_Col: ");
	  //ui_globals.action = POSITIVE;
	}
    }

  else if (strncmp(cmd, "r_spike", 7) == 0)
    {
      if (nargs == 3)
	{ 
	  do_r_spike(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the Col and Row number to remove the spike");
	  set_cmd_prompt("Col, Row#: ");
	  //ui_globals.action = R_SPIKE;
	}
    }

  else if (strncmp(cmd, "mathint", 7) == 0 ||
	   strncmp(cmd, "math_int", 8) == 0)
    {
      if (nargs == 10)
	{ 
	  do_mathint(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the math command: X-col $ Y-col @ New_col");
	  set_cmd_prompt("X-col, $, Y-col, @, New_Col: ");
	  //ui_globals.action = MATHINT;
	}
    }
  
  else if (strncmp(cmd, "math", 4) == 0)
    {
      if (nargs == 8)
	{ 
	  do_math(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the math command: X-col $ Y-col @ New_col");
	  set_cmd_prompt("X-col, $, Y-col, @, New_Col: ");
	  //ui_globals.action = MATH;
	}
    }
  
  else if (strncmp(cmd, "interpolate", 11) == 0)
    {
      if (nargs == 12)
	{ 
	  do_interpolate(input);
	}
      else
	{
	  if (cmd[strlen((char *)cmd)-1] == 'h')
	    {
	      sprintf(msg, "Mid point interpolation for y.\nRows from the end of interpolated data can be removed using interpolate_r.\n");
	      if(verbose == 'y')
	      	print_msg(msg);
	      }

	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the x-col, y-col, new x-col and new y-col");
	  set_cmd_prompt("X-col, Y-col, New_X-col, New_Y-col: ");

	  //if (cmd[strlen((char *)cmd)-1] == 'r')
	    //ui_globals.action = INTERPOLATE_R;
	  //else	  
	    //ui_globals.action = INTERPOLATE;
	}
    }

  else if (strncmp(cmd, "sort", 4) == 0 || strncmp(cmd, "order", 5) == 0)
    {
      if (nargs == 4)
	{ 
	  do_sort(input);
	}
      else
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type Col, Start_row and End_row");
	  set_cmd_prompt("Col, Start, End: ");
	  //ui_globals.action = SORT;
	}
    }

  else if (strncmp(cmd, "polyfit", 7) == 0)
    {
      if (nargs == 11 || nargs == 10)	/*it will be 11 if fit is extended --the extra arg being a row number in position 9, do_polyfit --in cmd.c will deal with this*/
	{ 
	  do_polyfit(input);
	}
      
      else
	{
	  sprintf(msg, "Polynomial fit of order n.\n");
	  if(verbose == 'y')
	  	print_msg(msg);

	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X-col, Y-col, New_col, Order");
	  set_cmd_prompt("X-col, Y-col, New_col, Order: ");

	  //if (strncmp(cmd, "polyfit_i", 9) == 0)
	    //ui_globals.action = POLYFIT_I;
	  //else
	    //ui_globals.action = POLYFIT;
	}
    }

  else if (strncmp(cmd, "cs", 2) == 0 || strncmp(cmd, "strain", 6) == 0)
    {
      if (nargs == 8)
	{ 
	  do_cs(input);
	}
      
      else
	{
	  sprintf(msg, "Incremental shear strain calculation.\n");
	  if(verbose == 'y')
	  	print_msg(msg);
	  
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the axial_disp col, layer_thick col and output col");
	  set_cmd_prompt("Axial_disp, Layer_thick, Output Col: ");
	  //ui_globals.action = CS;
	}
    }

  else if (strncmp(cmd, "ec", 2) == 0)
    {
      if (nargs == 9)
	{ 
	  do_ec(input);
	}
      else
	{
	  sprintf(msg, "Correct displacement for elastic distortion within piston and sample column.\nThe algorithm builds a new displacement col. by summing intial disp. and incremental actual displacements  (measured - elastic).\n");
	  if(verbose == 'y')
	  	print_msg(msg);

	  if (nargs > 1)
	    stripper(input, 1);
	
	  set_left_footer("Type the disp. col, force (or stress) col and new col");
	  set_cmd_prompt("disp_col, force_col, new_col: ");
	  //ui_globals.action = EC;
	}
    }

  else if (strncmp(cmd, "cgt", 3) == 0 || strncmp(cmd, "calc_geometric_thinning", 23) == 0)
    {
      if (nargs == 7)
	{ 
	  do_cgt(input);
	}
      else
	{
	  if (cmd[strlen((char *)cmd)-1] == 'h')
	    {
	      sprintf(msg,"Calculate geometric thinning\n  calculation is: (1)  del_h = h dx/2L ; where h is input thickness, dx is slip increment and L is length of the sliding block parallel to slip \n");
	      if(verbose == 'y')
	      	print_msg(msg);	    
	      sprintf(msg, "(1) Assumes an h appropriate for *one* layer (not both!) and thus the calculation is for one layer\n");
    	 	if(verbose == 'y')
	      		print_msg(msg);	    
	      sprintf(msg, "Also, make sure your equation is dimensionally correct!     -see also rgt\n");
    	 	if(verbose == 'y')
	      		print_msg(msg);
	    }
	  
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the Length, initial_h, Disp_col and New_col");
	  set_cmd_prompt("L, h, Disp_col, New_col: ");
	  //ui_globals.action = CGT;
	}
    }
  
  else if ((strncmp(cmd, "rgt", 3) == 0) || (strncmp(cmd, "geometric_thinning", 18) == 0))
    {
      if (nargs == 7)
	{ 
	  do_rgt(input);
	}
      else
        { 
          if(cmd[strlen((char *)cmd)-1] == 'h')
	    {
	      sprintf(msg, " Correct horizontal displacement measurement during direct shear test for geometric thinning\n  correction is: (1)  del_h = h dx/2L ; where h is thickness, dx is slip increment and L is length of the sliding block parallel to slip \n"); 
    	 	if(verbose == 'y')
	      		print_msg(msg);
	      sprintf(msg, "(1) Assumes an h appropriate for *one* layer (not both!) and thus the H. disp. measurements being corrected should be for one layer\nAlso, make sure your equation is dimensionally correct!   --see also cgt\n");
    	 	if(verbose == 'y')
	      		print_msg(msg);
	    }
	  
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the Length, Disp_col, Gouge_thickness col and New_col");
	  set_cmd_prompt("L, Disp_col, Thick_col, New_col: ");
	  //ui_globals.action = RGT;
	}
    }

  else if ((strncmp(cmd, "vc", 2) == 0) || (strncmp(cmd, "vol_cor", 7) == 0))
   {
     if (nargs == 9)
	{ 
	  do_vc(input);
	}
      else
        {
	  if(cmd[strlen((char *)cmd)-1] == 'h')
	    {
	      sprintf(msg, "Corrrect porosity/volume strain (during loading/unloading) for the effect of confining pressure on the material at the edges of the layer.\n");
    	 	if(verbose == 'y')
	      		print_msg(msg);
	      sprintf(msg, "User provides a compressibility (dv/V/dP) * the ratio of the affected volume (eg. at the edges) to the total volume.\n");
    	 	if(verbose == 'y')
	      		print_msg(msg);
	      sprintf(msg, "A first guess for the volume of material affected by Pc during load/unload is an anular -elliptical area- of width = layer thickness * thickness\n\n"); 
    	 	if(verbose == 'y')
	      		print_msg(msg);
	    }
	  
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the Vol strain, Shear stress col and New col");
	  set_cmd_prompt("Vol_strain, Shear_stress_col, New_col: ");
	  //ui_globals.action = VC;
      	}
   }
  
  else if ((strncmp(cmd, "deriv", 5) == 0))
    {
      if (nargs == 8)
	{ 
	  do_deriv(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col, Y col and New col for dy/dx derivative");
	  set_cmd_prompt("X-col, Y-col, New-col: ");
	  //ui_globals.action = DERIV;
      	}
    }

  else if ((strncmp(cmd, "exp", 3) == 0))
    {
      if (nargs == 5)
	{ 
	  do_exp(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col and New col");
	  set_cmd_prompt("X-col, New-col: ");
	  //ui_globals.action = EXP;
      	}
    }

  else if ((strncmp(cmd, "ln", 2) == 0))
    {
      if (nargs == 5)
	{ 
	  do_ln(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col and New col");
	  set_cmd_prompt("X-col, New-col: ");
	  //ui_globals.action = LN;
      	}
    }

  else if ((strncmp(cmd, "Power1", 6) == 0))
    {
      if (nargs == 7)
	{ 
	  do_Power1(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	  set_left_footer("Type the X col, New col, A and B");
	  set_cmd_prompt("X-col, New-col, A, B: ");
	  //ui_globals.action = POWER1;
      	}
    }

  else if ((strncmp(cmd, "Power2", 6) == 0))
    {
      if (nargs == 8)
	{ 
	  do_Power2(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col, New col, A, B, C");
	  set_cmd_prompt("X-col, New-col, A, B, C: ");
	  //ui_globals.action = POWER2;
      	}
    }

  else if ((strncmp(cmd, "normal", 6) == 0))
    {
      if (nargs == 7)
	{ 
	  do_normal(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col, New col, mean an stddev");
	  set_cmd_prompt("X-col, New-col, Mean, Stddev: ");
	  //ui_globals.action = NORMAL;
      	}
    }

  else if ((strncmp(cmd, "chisqr", 6) == 0))
    {
      if (nargs == 6)
	{ 
	  do_chisqr(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col, New col and ndf");
	  set_cmd_prompt("X-col, New-col, ndf: ");
	  //ui_globals.action = CHISQR;
      	}
    }

  else if ((strncmp(cmd, "scchisqr", 8) == 0))
    {
      if (nargs == 8)
	{ 
	  do_chisqr(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col, New col, sigma, ndf and offset");
	  set_cmd_prompt("X-col, New-col, sigma, ndf, offset: ");
	  //ui_globals.action = SCCHISQR;
      	}
    }

  else if ((strncmp(cmd, "rclow", 5) == 0))
    {
      if (nargs == 7)
	{ 
	  do_rclow(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col, New col, sigma, A and B");
	  set_cmd_prompt("X-col, New-col, A, B: ");
	  //ui_globals.action = RCLOW;
      	}
    }

  else if ((strncmp(cmd, "genexp", 6) == 0))
    {
      if (nargs == 9)
	{ 
	  do_genexp(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col, New col, A, B, C and D");
	  set_cmd_prompt("X-col, New-col, A, B, C, D: ");
	  //ui_globals.action = GENEXP;
      	}
    }

  else if ((strncmp(cmd, "gensin", 6) == 0))
    {
      if (nargs == 9)
	{ 
	  do_gensin(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col, New col, A, B, C and D");
	  set_cmd_prompt("X-col, New-col, A, B, C, D: ");
	  //ui_globals.action = GENSIN;
      	}
    }

  else if ((strncmp(cmd, "Poly4", 5) == 0))
    {
      if (nargs == 10)
	{ 
	  do_Poly4(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	  set_left_footer("Type the X col, New col, A, B, C, D and E");
	  set_cmd_prompt("X-col, New-col, A, B, C, D, E: ");
	  //ui_globals.action = POLY4;
      	}
    }

  else if ((strncmp(cmd, "ExpLin", 6) == 0))
    {
      if (nargs == 8)
	{ 
	  do_ExpLin(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col, New col, A, B and C");
	  set_cmd_prompt("X-col, New-col, A, B, C: ");
	  //ui_globals.action = EXPLIN;
      	}
    }

  else if ((strncmp(cmd, "log", 3) == 0))
    {
      if (nargs == 5)
	{ 
	  do_log(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col and New col");
	  set_cmd_prompt("X-col, New-col: ");
	  //ui_globals.action = LOG;
      	}
    }

  else if ((strncmp(cmd, "recip", 5) == 0))
    {
      if (nargs == 5)
	{ 
	  do_recip(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);

	  set_left_footer("Type the X col and New col");
	  set_cmd_prompt("X-col, New-col: ");
	  //ui_globals.action = RECIP;
      	}
    }

  else if ((strncmp(cmd, "power", 5) == 0))
    {
      if (nargs == 6)
	{ 
	  do_power(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	    
	  set_left_footer("Type the Power, X col and New col");
	  set_cmd_prompt("Power, X-col, New-col: ");
	  //ui_globals.action = POWER;
      	}
    }

  else if ((strncmp(cmd, "col_power", 9) == 0))
    {
      if (nargs == 6)
	{ 
	  do_col_power(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	  
	  set_left_footer("Type the Power col, X col and New col");
	  set_cmd_prompt("Power-col, X-col, New-col: ");
	  //ui_globals.action = COL_POWER;
      	}
    }

  else if ((strncmp(cmd, "rcph", 4) == 0))
    {
      if (nargs == 7)
	{ 
	  do_rcph(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	    
	  set_left_footer("Type the X col, New col, A and B");
	  set_cmd_prompt("X-col, New-col, A, B: ");
	  //ui_globals.action = RCPH;
      	}
    }


  else if (strncmp(cmd,"trig", 4) == 0 || 
	   strncmp(cmd,"sin", 3) == 0 || 
	   strncmp(cmd,"cos", 3) ==  0 || 
	   strncmp(cmd,"tan", 3) == 0 || 
	   strncmp(cmd,"asin", 4) == 0 ||
	   strncmp(cmd,"acos", 4) == 0 || 
	   strncmp(cmd,"atan", 4) == 0 )
    {
      strcpy(trig_cmd, cmd);
      
      if (nargs == 6)
	{ 
	  do_col_power(input);
	}
      else  
	{
	  if (nargs > 1)
	      stripper(input, 1);

	  set_left_footer("Type the Column, New col and Function");
	  set_cmd_prompt("Col, New-col, Func: ");
	  //ui_globals.action = TRIG;
	}
    }

  else if ((strncmp(cmd, "slope", 5) == 0) || (strncmp(cmd, "ras", 3) == 0) )  
    {
      /* used to be called o_slope */
      if (nargs == 9)
	{ 
	  do_slope(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	  
	  sprintf(msg, "Calculate running average slope, dY/dX, and put in \"New Col\"\n");
    	 	if(verbose == 'y')
	  		print_msg(msg);
	  sprintf(msg, "Window size is forced to be odd.\nEnds are padded with first reasonable value (half_window from start/end)\n");
    	 	if(verbose == 'y')
	  		print_msg(msg);
	  set_left_footer("Type the X col, Y Col, New Col, Start Row, End Row and Window size");
	  set_cmd_prompt("X-col, Y-Col, New-col, Start, End, Win-size: ");
	  //ui_globals.action = SLOPE;
      	}
    }

  else if ((strncmp(cmd, "rsm", 3) == 0))
    {
      if (cmd[strlen((char *)cmd)-1] == 'h')
	print_rsm_help_info();
   
      sprintf(msg, "One or two state variable friction model calculated at displacements specified by disp col.\n");
    	 if(verbose == 'y')
      		print_msg(msg);
      sprintf(msg, "rsm_h gives a detailed description of this function. See also cm_h \n");
    	 if(verbose == 'y')
      		print_msg(msg);
//      sprintf(msg, mu_fit_mess_1); // this variable was never set (rdm)
//      print_msg(msg);
      
      if (nargs == 16)
	{ 
	  do_rsm(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	  
	  sprintf(msg, "Enter: disp. col., mu col, row # of vel. step and end of data to model, col for model_mu,  stiffness (k), Sigma_n, v_initial (v_o), v_final, mu_o, mu_f, a, b1, Dc1, and Dc2\n");
    	  if(verbose == 'y')
	  	print_msg(msg);
	  set_left_footer("");
	  set_cmd_prompt("Cmds: ");
	  //ui_globals.action = RSM;
	}
    }

  else if ((strncmp(cmd, "cm", 2) == 0))
    {
      if (cmd[strlen((char *)cmd)-1] == 'h')
	print_cm_help_info();
      
      if (nargs == 17)
	{ 
	  do_cm(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	    
	  sprintf(msg, "Forward modelling of rate/state variable friction. (cm_h gives a detailed description of this function) \n");
    	  if(verbose == 'y')
	  	print_msg(msg);
	  sprintf(msg, "Enter: model_disp_col, (data) disp. col., (data) mu col,  vel_step_row, end_row, model_mu_col, \n\tstiffness (k), Sigma_n,v_initial (v_o), v_final, mu_o, mu_f, a, b1, Dc1, and Dc2\n");
	  	print_msg(msg);
	  sprintf(msg, "\tmodel_disp_col, disp_col, mu_col, vs_row, end_row, model_mu_col, k, Sigma_n, v_o, vf, mu_o, mu_f, a, b1, Dc1, Dc2 \n ") ;
	  	print_msg(msg);

	  set_left_footer("");
	  set_cmd_prompt("Inputs: ");
	  //ui_globals.action = CM;
	}
    }


  else if ((strncmp(cmd, "mem", 3) == 0))
    {
      if (cmd[strlen((char *)cmd)-1] == 'h')
	{
	  sprintf(msg, "Command line interpretation: \n ycol = signal for which you want power spectra.\n xcol = spacing of signal (i.e., time, dist)\n");
    	  if(verbose == 'y')
	  	print_msg(msg);
	  sprintf(msg,"  2 cols are output: freq. and power.\n the spectra is calculated between records 'first_row' and 'last row'\n  #freqs is the # of frequencies  (beginning at the nyquist) at which to calculate power.\n");
    	  if(verbose == 'y')
	  	print_msg(msg);
	  sprintf(msg,"  and #poles is the # of poles to use.   The frequencies can either be distributed linearly (n) or \n logarithmically (l=> linear on a log plot), a welsh taper may be applied to the data (w)\n\n");
    	  if(verbose == 'y')
	  	print_msg(msg);
	  top();
          ////ui_globals.action = MAIN;
          return;
	}
      
      if (nargs == 11)
	{ 
	  do_mem(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	    
	  sprintf(msg, "Power spectra estimate using maximum entropy method, n=linear freq. series, l=log, w=welsh taper, n=no taper (mem_h  for help)\n");
    	  if(verbose == 'y')
	  	print_msg(msg);
	  sprintf(msg, "Inputs for mem: xcol, ycol, freq_col, power_col, first_row, last_row, #freqs, #poles, l/n, w/n\n") ;
    	  if(verbose == 'y')
	  	print_msg(msg);
	  set_left_footer("");
	  set_cmd_prompt("Inputs: ");
	  //ui_globals.action = MEM;
	}
    }


  else if ((strncmp(cmd, "median_smooth", 13) == 0) ||
	   (strncmp(cmd, "ms", 2) == 0))
    {
      if (cmd[strlen((char *)cmd)-1] == 'h')
	{ 
	  sprintf(msg, "Calculate the running average *median* value of COL within a moving window between start_row and end_row.\n\t*the length of `window' is forced to be odd*\n");
    	  if(verbose == 'y')
	 	 print_msg(msg);
	}
      
      if (nargs == 8)
	{ 
	  do_median_smooth(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	    
	  sprintf(msg, "Inputs for median smooth: Col, New_Col , Start_Row, End_Row, Window_Size\n") ;
    	  if(verbose == 'y')
	  	print_msg(msg);
	  set_left_footer("");
	  set_cmd_prompt("Inputs: ");
	  //ui_globals.action = MEDIAN_SMOOTH;
	}
    }


  else if ((strncmp(cmd, "smooth", 6) == 0))
    {
      if (nargs == 8)
	{ 
	  do_smooth(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	    
	  sprintf(msg, "Inputs for smooth: Col, New_Col , Start_Row, End_Row, Window_Size\n") ;
    	  if(verbose == 'y')
	  	print_msg(msg);
	  set_left_footer("");
	  set_cmd_prompt("Inputs: ");
	  //ui_globals.action = SMOOTH;
	}
    }
  
  else if ((strncmp(cmd, "typeall", 7) == 0) || 
	   (strncmp(cmd, "type_all", 8) == 0))
    {
      if (nargs == 2)
	{ 
	  do_typeall(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);
	    
	  set_left_footer("Input the filename or type 'S' for screen output");
	  set_cmd_prompt("Input: ");
	  //ui_globals.action = TYPEALL;
	}
    }
  
  else if ((strncmp(cmd, "type", 4) == 0))
    {
      if (nargs == 6)
	{ 
	  do_type(input);
	}
      else  
	{
	  sscanf(cmd, "%s", type_cmd);
	  
	  if (nargs > 1)
	    stripper(input, 1);
	    
	  set_left_footer("Input the starting and ending record numbers");
	  set_cmd_prompt("Start End: ");
	  //ui_globals.action = TYPE;
	}
    }

  else if ((strncmp(cmd, "stat", 4) == 0))
    {
      if (nargs == 4 || ((strncmp(cmd, "stat_a", 6) == 0) && nargs == 2) )
	{ 
	  do_stats(input);
	}
      else  
	{
	  if (nargs > 1)
	    stripper(input, 1);	 	/*this is in strcmd.c*/

	  if(strncmp(cmd, "stat_a", 6) == 0)
	  {
		set_left_footer("Type the col number");
	  	set_cmd_prompt("Col: ");
	  	//ui_globals.action = STAT_A;
	  }
	  else
	  {
		set_left_footer("Type the starting and ending record numbers");
	  	set_cmd_prompt("Col Start End: ");
	  	//ui_globals.action = STAT;
	  }
	}
    }

  else if (strncmp(cmd, "qi", 2) == 0)
  {
      if(cmd[(strlen((char *)cmd)-1)] == 'h')
      {
	  do_qi_help();
          top();
          ////ui_globals.action = MAIN;
          return;
      }
      else if (nargs == 21)
	  do_qi(input);
      else 
      { 
	  if (nargs > 21)
	  {  
	    stripper(input,21);
	    do_qi(input);
	  }
      	  else 
	  {
	    strcpy(qi_cmd, cmd);
	  
	    sprintf(msg, "Linearized inversion of one or two state variable friction model.\n\t(qi_h gives a detailed description of this function) \n");
    	  if(verbose == 'y')
	    print_msg(msg);
	    sprintf(msg, "\nNeed disp. col, mu col, model_mu col, beginning row for fit, row # of vel. step, end row, weight_row, lin_term(c), converg_tol, lambda,  wc, stiffness, v_initial, v_final, mu_init, a, b1, Dc1, b2 and Dc2 for input.\n ");
    	  if(verbose == 'y')
	    print_msg(msg);
	    sprintf(msg, "Input:\nd_col, mu_col, mod_col, first_row, vs_row, end_row, weight_row, c, tol, lambda, wc, k, v_o, vf, mu_o, a, b1, Dc1, b2, Dc2 \n");
    	  if(verbose == 'y')
	    print_msg(msg);
	    set_left_footer("");
	    set_cmd_prompt("QI Input: ");
	    //ui_globals.action = QI;
	  }
      }
  }
  else if (strncmp(cmd, "scm", 3) == 0) 
    {
      if (nargs > 16)       
	{
	  stripper(input, 16);
	  do_scm(input); 	 	
	} 
      else 	 	
	{ 
	  if ((strlen(cmd) >= 5) && (strncmp(cmd, "scm_h", 5) == 0))
	    do_scm_help();
	  do_scm_info();
	  set_cmd_prompt("Input: ");
	  set_left_footer("Type all the arguments for scm");
	  //ui_globals.action = SCM_GET_ARGS; 	
	}
    }


}


