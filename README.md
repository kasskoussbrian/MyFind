How to run the project:
-
Enter the myfind directory and run make . <br />

This is the typical usecase  :   

./myfind [starting-point...] [expressions]



These are the options implemented in this project : 
-
-print :
prints the path of the currently examined file. It always returns true<br />
-name  : takes a pattern as parameter and returns true if the current filename matches the parameter.
You must handle globbing<br /><br />
-type  : takes a parameter and returns true if the type of the current file matches the parameter. You
must handle the following types:<br />
• b: special files in block mode<br />
• c: special files in character mode<br />
• d: directories<br />
• f: regular files<br />
• l: symbolic links<br />
• p: named pipes (FIFO)<br />
• s: sockets<br /><br />
-newer : takes a file as parameter and returns true if the currently-examined file has a last modification
date more recent than the file given as argument <br />
-perm  : takes a mode in octal as parameter and returns true if the current file’s permission bits match
exactly the mode. | works with - and /<br />
-user  : takes a username and returns true if the file is owned by the user username.<br />
-group : takes a groupname and returns true if the file belongs to the group groupname<br />

-delete: deletes files and returns true if it succeeded. If the deletion fails, an error message is dis-
played. <br />!!!   BE CAREFULL WHEN USING THIS OPTION AS THE CODE MIGHT HAVE SOME BUGS IN IT   !!!<br />

-a : represents the and operator<br />
-o : represents the or operator<br />
! : represents the not operator<br />
