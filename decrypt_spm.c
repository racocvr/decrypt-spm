/*
	decrypt .spm - MrB 2021
	
	A command line tool to decrypt .spm files on a Samsung smart tv.
	
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int appdrm_load_license(const char *license_file, void **license_handle);
int appdrm_release_license(void **license_handle);
int appdrm_decrypt_spm(void **license_handle, const char *filename, unsigned char **outbuf, int *outlen);

int main(int argc, char **argv)
{
	if(argc < 3)
	{
		printf("usage: %s <license_file> <spm_file>\n", argv[0]);
		exit(1);
	}
			
	unsigned char* outbuf;
	int32_t outlen = 0;			
	char outfile[256];
	void *license_handle;
	
	if( appdrm_load_license(argv[1], &license_handle) < 0 )
	{
		printf("error reading license_file\n");
		return -1;
	}
	
	if( appdrm_decrypt_spm(&license_handle, argv[2], &outbuf, &outlen) < 0 )
	{
		printf("error decrypting spm file\n");
		return -1;
	}
	
	if( appdrm_release_license(&license_handle) < 0 )
	{
		printf("error releasing license_handle\n");
		return -1;
	}

	sprintf( outfile, "%s", argv[2] );
	outfile[strlen(argv[2]) - 4] = 0;	
	
	printf("writing to %s\n", outfile);	
	
	FILE *write_ptr = fopen(outfile,"wb");
	if( write_ptr == NULL )
	{
		printf("error opening output file\n");
		return -1;
	}
	
	fwrite( outbuf, outlen, 1, write_ptr );
	fclose( write_ptr );
		
    return 0;
}