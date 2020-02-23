#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

struct mFILE {
	int fd;	
};

mFILE* mfopen(const char *fn, const char *mode){
	mFILE* mfile = (mFILE*) malloc(sizeof(mFILE));
	int flag = 0;
	mode_t perms = S_IRUSR | S_IWUSR;
	if(strcmp(mode,"w") == 0){
		flag =  O_WRONLY | O_CREAT | O_TRUNC;
	}
	else if(strcmp(mode, "a") == 0) {
		flag = O_WRONLY | O_CREAT | O_APPEND;
	}
	else if(strcmp(mode, "r") == 0) {
		flag = O_RDONLY;
	}
	else if(strcmp(mode, "r+") == 0) {
		flag = O_RDWR;
	}
	else if(strcmp(mode, "w+") == 0) {
		flag = O_RDWR | O_CREAT | O_TRUNC; 
	}
	else if(strcmp(mode, "a+") == 0) {
		flag = O_RDWR | O_CREAT | O_APPEND; 
	}
	mfile->fd = open(fn, flag, perms);
	if(mfile->fd == -1){
		return NULL;
	}	
	return mfile;		
}

int mfread(void *ptr, size_t size, size_t nitems, mFILE *fptr){
	return read(fptr->fd, ptr, size*nitems);

}

int mfwrite(void *ptr, size_t size, size_t nitems, mFILE *fptr){
	return write(fptr->fd, ptr,size*nitems);
}

int mfclose(mFILE *fptr){
	free(fptr);	
	return close(fptr->fd); 
}

void mperror(){
	mFILE fpse;
	fpse.fd = 2;
	char e[28] = "YOU DON'T HAVE PERMISSION.\n";
	mfwrite(e, 27, 1, &fpse);
	exit(1);
}

void mferror(){
	mFILE fpse;
	fpse.fd = 2;
	char e[13] = "FILE ERROR.\n";
	mfwrite(e, 12, 1, &fpse);
	exit(1);
}
int main(int argc, char **argv){
//w
	mFILE *fp = mfopen("a.csv", "w");	
	if(fp == NULL){
		mferror();
	}
	char c[7] = "write\n";
	if(mfwrite(&c, 6, 1, fp) == -1){
		mperror();	
	}
	int clos = mfclose(fp);
	if (clos == -1){
		mferror();
	}
//r
	fp = mfopen("a.csv", "r");
	if(fp == NULL){
		mferror();
	}
	mFILE fpso;
	fpso.fd = 1;	
	char r;
	while(true){
		int e = mfread(&r, 1, 1, fp);
		if(e == -1){
			mperror();
		}
		else if (e == 0){
			break;
		}
		if(mfwrite(&r, 1, 1, &fpso) == -1){
			mperror();
		}
	}
	clos = mfclose(fp);
	if (clos == -1){
		mferror();
	}

//a
	fp = mfopen("a.csv", "a");
	if (fp == NULL) {
		mferror();
	}
	char a[8] = "Append\n";
	if(mfwrite(&a, 7, 1, fp) == -1){
		mperror();
	}
	clos = mfclose(fp);
	if (clos == -1){
		mferror();
	}

//r+	
	fp = mfopen("a.csv", "r+");	
	if (fp == NULL) {
		mferror();
	}
	while(true){
		int e = mfread(&r, 1, 1, fp);
		if(e == -1){
			mperror();
		}
		else if (e == 0){
			break;
		}
		if(mfwrite(&r, 1, 1, &fpso) == -1){
			mperror();
		}
	}
	lseek(fp->fd, 0, SEEK_SET);
	char rp[10] = "readplus\n";
	if (mfwrite(&rp, 9, 1, fp) == -1){
		mperror();
	}
	clos = mfclose(fp);
	if (clos == -1){
		mferror();
	}

//w+
	fp = mfopen("a.csv", "w+");
	if (fp == NULL) {
		mferror();
	}
	char w[11] = "Writeplus\n";
	if(mfwrite(&w, 10, 1, fp) == -1){
		mperror();
	}
	lseek(fp->fd, 0, SEEK_SET);	
	while(true){
		int e = mfread(&r, 1, 1, fp);
		if(e == -1){
			mperror();
		}
		else if (e == 0){
			break;
		}
		if(mfwrite(&r, 1, 1, &fpso) == -1){
			mperror();
		}
	}	
	clos = mfclose(fp);
	if (clos == -1){
		mferror();
	}
//a+
	fp = mfopen("a.csv", "a+");
	if (fp == NULL) {
		mferror();
	}	
	char ap[12] = "appendplus\n";
	if(mfwrite(&ap, 11, 1,fp) == -1){
		mperror();
	}
	clos = mfclose(fp);
	if (clos == -1){
		mferror();
	}
}
