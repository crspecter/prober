#include "unistd.h"
#include "stdlib.h"
#include "daemon.h"
#include "master_cluster.h"

#include "log.h"






int main(int argc , char *argv[])
{

	init_dae  dae;
	dae.parse_cmd_line(argc, argv);
	
	master_cluster master;
	master.start_up();
	
	for(;;)
	{	
		sleep(1);
	}
	return 0;	
}
