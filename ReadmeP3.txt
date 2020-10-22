To run

make clean
	This will clean up any object files already created and remove them

make
	This will compile and create a directory of bin/ with the executable code stored in it.
	View USE on how to run these programs in the bin/

USE (executables are stored in /bin):

	oss [-h] [-c x] [-l filename] [-t z]

	Where:
		-c x: 		Controls how many children you are allowing to spawn (default 5)
		-l filename: 	Log file you want to create
		-t z: 		Controls how long before the system will timeout (default 20)

	-l filename is the only REQUIRED parameter to run, there is no default file created

