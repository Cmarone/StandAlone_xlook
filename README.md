//This is a stand alone version of xlook. It doesn't have graphics. It's meant to run "_r file" scripts and produce ASCII output for plotting.
_r files were called by the 'doit' command in xlook.
doit files can be nested
The pathnames for nesting is set up in cmds.c

doit files work by a procedure called in doit_proc() in cmds1.c
doit_proc reads lines, does some error checking and sends the command to command_handler() in event.c

recall that all of the commands are in command_handler in event.c
After the command_handler there is process_action(arg). This was the way things were processed from the xlook command line in X and GTK. It's a big switch(ui_globals.action) with all the commands. It does nothing for doit files...

This is meant to run from a unix command line.

You can produce the executable using the make file as in: make saxl
You'll need the gcc compiler
All of the files should be in one directory

There's a usage file. Just type saxl and you'll see it.  Make sure the program is in a directory that's in your path --or you can type ../saxl 

Here's the usage info:
      fprintf(stderr,"\t\n This is a stand alone version of xlook. It doesn't have graphics. It's meant to run so-called _r file scripts and produce ASCII output. \n" );
        fprintf(stderr, "Usage is: %s filename -v,w, where filename is an xlook 'doit file' script\n You can find an example of a script and other xlook details on github here \n", progname);
        fprintf(stderr,"-v = verbose option writes every command from doit file to stderr\n");
        fprintf(stderr,"-w = at the end, write the whole file as ascii using filename.txt \n");

There's an example data file and r file. p655_r and p655intact100l

Check it out as: saxl p655_r

If you run into trouble with other r files check the read statement. Most of the old ones expected the data file to be in the directory, so have some complicated path.
Put the data file in the directory you're working in and change the read file to read it directly...

Have fun
cjm: 10 March 2024
