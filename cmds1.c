#include <string.h>
#include <unistd.h>
#include <sys/file.h>

#include "cmds1.h"
#include "config.h"
#include "event.h"
#include "filtersm.h"
#include "global.h"
#include "look_funcs.h"
#include "messages.h"
#include "notices.h"
#include "special.h"
#include "strcmd.h"

/* cjm 14.5.07; to solve problem with doit files: increased the size of path names. 1024's used to be 80, 512 used to be 32 */
char pathname[10][1024];
char default_path[1024];
char metapath[1024];
char data_file[512];
char new_file[512];
char headline[512];

FILE *data, *new;

int doit_des, doit_f_open, meta_fd;
int plot_error;
int read_flag = 0; 

extern char plot_cmd[256], trig_cmd[256];

extern char msg[MSG_LENGTH];
extern char    verbose, write_all_at_end;


void write_proc( char arg[])
     //char arg[256];
{
  
  strcpy(new_file, arg);

  if ((new = fopen(new_file, "ab")) == NULL)
    {
      sprintf(msg, "Can't open file: %s. fopen in write_proc() failed. See cmds1.c\n", new_file);
      print_msg(msg);
      //ui_globals.action =MAIN;
      top();
      return;  
    }
  else
    {
      /* file already exist */
      if ((int)ftell(new) > 0)
	{
	  /* ask if want to overwrite old file */
	  if (1  != 1)
	  //if (write_show_warning() != 1)
	    {
	      /* don't overwrite */
	      sprintf(msg, "Write aborted!\n");
	      print_msg(msg);
	      //ui_globals.action =MAIN;
	      top();
	      return;
	    }
	  else
	    {
	      if ((new = fopen(new_file, "wb")) == NULL)
		{
		  sprintf(msg, "Can't open file: %s.\n", new_file);
		  print_msg(msg);
		  //ui_globals.action =MAIN;
		  top();
		  return;
		}
	      else
		{
		  sprintf(msg, "Overwriting %s.\n", new_file);
		  print_msg(msg); 
		}
	    }
	}

      if (act_col() > head.nchan)
	{
	  sprintf(msg, "Active columns = %d\n", act_col());
	  print_msg(msg);
	  sprintf(msg, "Please compact data array to use columns 1 through head.nchan[%d]\n", head.nchan);
	  print_msg(msg);
	  sprintf(msg, "Reallocation is not necessary.\n");
	  print_msg(msg);
	  //ui_globals.action =MAIN;
	  top();
	  return;	
	}
      
      rite_lookfile(new);
      fclose(new);
      sprintf(msg, "File %s written. \n", new_file);
      print_msg(msg);
      //ui_globals.action =MAIN;
      top(); 
    }
}


void read_proc(const char cmd[256])
{
	char dummy[256];
	char other[256];
	int ii =  0;
	int count = 0;

	sscanf(cmd, "%s %s", dummy, data_file);

	strcpy(other, data_file); 	//check to see if it's an old r file with a long path...
	while (other[ii] != '\0') {			//maybe it's an old r file with a long path..
       	// If the current character is '/', increment the count
       	  if (other[ii] == '/') {
       		count++;
       	  }
          ++ii;
    	}

	for(ii = 0; ii < count; ii++)	  //find the last /  The file name will be next
		sscanf(other, "%*[^/]/%255s", other);

	fprintf(stderr,"here and: %s\n",other);

	if( (data = fopen(other, "rb")) != NULL) 
	{
		sprintf(msg, "Reading %s...\n", other);
		print_msg(msg);

		/*  reed is in filtersm.c   */
		if (reed(data, ((strncmp(dummy, "append", 6) == 0) ? TRUE : FALSE)) != 1)
		{
			display_active_file(0);
			fclose(data);
			//ui_globals.action = MAIN;
			top();
			return;
		}
		display_active_file(1);
		fclose(data);
		sprintf(msg, "Reading %s done.\n", data_file);
		print_msg(msg);
		++read_flag; /* file successfully opened */
		strcpy(data_file, other); 	//put short file name in 'data_file' 
		//ui_globals.action = MAIN;
		top();
	}
	else if( (data = fopen(data_file, "rb")) != NULL)
	{
		sprintf(msg, "Reading %s...\n", data_file);
		print_msg(msg);

		/*  reed is in filtersm.c   */
		if (reed(data, ((strncmp(dummy, "append", 6) == 0) ? TRUE : FALSE)) != 1)
		{
			display_active_file(0);
			fclose(data);
			//ui_globals.action = MAIN;
			top();
			return;
		}
		display_active_file(1);
		fclose(data);
		sprintf(msg, "Reading %s done.\n", data_file);
		print_msg(msg);
		++read_flag; /* file successfully opened */
		//ui_globals.action = MAIN;
		top();
	}
	else 
	{

		sprintf(msg, "Can't open data file: %s. Tried %s too.  Check filename and location.. This code is in cmds1.c\n", data_file, other);
		fprintf(stderr,"%s\n",msg);
		print_msg(msg);
		//ui_globals.action = MAIN;
		top();
		return;
	}
}
  

void doit_proc(const char arg[256])
{
	/* reads a doit file and passes the commands to command_handler() directly */  
	//char *fgets();
	char cmd[256];
	int i, j, error;


	if ((doit_f_open+1) > 9)
	{
		sprintf(msg, "Sorry but you can only nest up to 10 doit files.\n");
		print_msg(msg);
		//ui_globals.action=MAIN;
		top();
		return;
	}

	/* need to set action to MAIN here so that the commands in doit file can use action */
	//ui_globals.action = MAIN;

	strcpy(pathname[doit_f_open+1], default_path);  	/*set up file path name*/
	strcat(pathname[doit_f_open+1], arg);
	/*strcpy(data_file, arg);
	strcat(pathname[doit_f_open+1], data_file);*/

	/* open file, do some initial checking, then add to array of open doit files*/
	if ( (temp_com_file = fopen(pathname[doit_f_open+1], "r")) == NULL)
	{
		sprintf(msg, "Open failed- can't open data file: \"%s\"\n",  pathname[doit_f_open+1]);
		print_msg(msg);
		//ui_globals.action = MAIN;
		top();
		return;
	}
	else
	{
		if (fscanf(temp_com_file, "%s", cmd) != 1)
		{
			sprintf(msg,"Cannot read file.\n");
			print_msg(msg);
			fclose(temp_com_file);
			//ui_globals.action = MAIN;
			top();
			return;
		}
		if (strncmp(cmd, "begin", 5) != 0)
		{
			sprintf(msg, "doit file must begin with the string: begin\n");
			print_msg(msg);
			fclose(temp_com_file);
			//ui_globals.action = MAIN;
			top();
			return;
		}

		/* loop thru the file until EOF; read the file line by line, put the string (whole line) in cmd
		pass cmd to command_handler() */

		doit_f_open++;				/*add to list of open doit files*/
		com_file[doit_f_open] = temp_com_file;  

		/*clear first line (should be "begin" at this point, report errors*/

		i=1;	/*use to count lines*/
		if( fgets(cmd, 256, com_file[doit_f_open]) == NULL)
		{
			sprintf(msg, "Error from doit file \"%s\", unexpected EOF on line %d, see cmds1.c \n",  pathname[doit_f_open+1], i);
			print_msg(msg);
			fclose(com_file[doit_f_open]);
			if (--doit_f_open < 0)
				doit_f_open = 0;
			//ui_globals.action = MAIN;
			top();
			return;
		}

		/* the global_error flag is set in messages.c when the error functions are called*/
		while( fgets(cmd, 256, com_file[doit_f_open]) != NULL && global_error == FALSE)
		{
			/*read a line from doit file*/
			i++;				/*increment line counter*/
			if( strlen(cmd) == 0)
			{
				sprintf(msg, "Error from doit file \"%s\", unexpected EOF on line %d, see cmds1.c \n",  pathname[doit_f_open+1], i);
				print_msg(msg);
				fclose(com_file[doit_f_open]);
				if (--doit_f_open < 0)
					doit_f_open = 0;
				//ui_globals.action = MAIN;
				top();
				return;
			}

			/*make sure line returned by fgets was "whole" --i.e. contained a NEWLINE, otherwise */
			/*lines may be longer than 256 (too long)*/
			j=0; error=TRUE;
			while(j < 256)			
			{
				if(cmd[j] == '\n')		/*find end of lines and replace with nulls*/
				{
					cmd[j] = '\0';
					error=FALSE;
					break;	
				}
				j++;
			}
			if(error)
			{
				sprintf(msg, "Error from doit file \"%s\", line %d too long. Lines must be < 256 chars. But this should be easy to fix, see cmds1.c \n",  pathname[doit_f_open+1], i);
				print_msg(msg);
				fclose(com_file[doit_f_open]);
				if (--doit_f_open < 0)
					doit_f_open = 0;
				//ui_globals.action = MAIN;
				top();
				return;
			}

			if (strncmp(cmd, "#", 1) != 0)  /*comments begins with # */
			{
				sprintf(msg, "Command: %s\n", cmd);
				if(verbose == 'y')			/*this is defined as global in saxl.c  --sorry, I know that's lazy*/
					print_msg(msg);
			}

			if (strncmp(cmd, "end", 3) == 0)
			{
      					/*cjm, so that we can use orig filename 20240304*/
			 	if(write_all_at_end == 'y')	//write file to ascii before quitting
			 	{
			    		strcpy(msg,data_file);
			  		strcat(msg, ".txt");

			    		strcpy(cmd,"typeall");	//type command is in cmds.c, do_type_final()  calls msg_p_vision()
			  		strcat(cmd, " ");
			  		strcat(cmd, msg);
				
			  		//fprintf(stderr,"doit cmd is %s\n",cmd);

					command_handler(cmd);
			  	}

				sprintf(msg, "Closing doit File %s\n",pathname[doit_f_open]);
				print_msg(msg);
				fclose(com_file[doit_f_open]);
				if (--doit_f_open < 0)
					doit_f_open = 0;
				break;
			}
			else if (strncmp(cmd, "#", 1) == 0) /*because comments begins with #, so ignore them*/
			{
				continue;
			}
			else 
			{
				command_handler(cmd);
			}
		}
		cmd[0] = '\0';	/*empty cmd buffer*/
		//ui_globals.action = MAIN;
		top();
	}
}

     

void set_path_proc( char arg [])  
//     char arg[256];
{
  /* change: if path is inaccessible, set path to current path instead of default path */
  
  /*  printf("path: %s  def_path: %s\n", arg, default_path); */
  
  if (access(arg, 4) != 0)
    {
      sprintf(msg, "Inaccessible path: %s.\n", arg);
      print_msg(msg);
    }
  
  else
    {
      strcpy(default_path, arg);
      if (default_path[strlen(default_path)-1] != '/')
	strcat(default_path, "/");
    }
  sprintf(msg, "Default path is %s\n", default_path);
  print_msg(msg);
  
  top();
  //ui_globals.action = MAIN;
}




void all_final_proc( char cmd[])
//     char cmd[256];
{
  char dummy1[256], dummy2[256];
  int i, j;
  
  if (sscanf(cmd, "%s %s %d %d", dummy1, dummy2, &i, &j) != 4)
    {
      print_msg("Error from all_final_proc: Input not recognized.\n");
      //ui_globals.action = MAIN;
      top();
      return;
    }
  
  if ((strcmp(dummy2, "yes") != 0) || (strcmp(dummy2, "y") != 0))
    {
      //ui_globals.action = MAIN;
      top();
      return;
    }
    
  if (j==0 || j>=MAX_COL || i==0 || i>max_row) 
    {
      sprintf(msg, "Illegal allocation: 0 < NROW < %d < NCOL < %d\n", max_row, MAX_COL);
      print_msg(msg);
      //ui_globals.action = MAIN;
      top();
      return;
    }
  
  allocate(i, j);
  sprintf(msg, "ALLOCATE: DONE\n");
  print_msg(msg);

  //ui_globals.action = MAIN;
  top();
  
}

