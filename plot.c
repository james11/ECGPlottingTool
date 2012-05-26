#include <stdio.h>
#include <cv.h>
#include <highgui.h>

void          example (void); 
IplImage      plot (double* data); 
unsigned long GetFileLength ( FILE * fileName);




int main(int argc, char *argv[ ]) 
{
    IplImage *Imagefinal;  
    CvSize ImageSize1 = cvSize(1000,800);
    Imagefinal = cvCreateImage(ImageSize1,IPL_DEPTH_8U,3);
    CvScalar Color;      
    Color=CV_RGB(255,0,0);  
    FILE *fp_scale;    
    FILE *fp_r;
    unsigned long len=0;   
    char scale[100];  
    char offset[100];  
    int filecount=0;  
    int t=0;  
    int i=0;
    int j=0;
    int ccc=0;
    double data[1920]={0};   
    int input=0;
    int in_res=0;
    int theshold=0;
    int flag_yes=0;
    int res=0;
    char* s1 =0; 
//    int resulation[10]={1000,2000,4000,8000,16000,32000,64000,128000,256000}; 
    int resulation[10]={1920,1920*5,1920*10,1920*15,1920*30}; 
    int time=0;

while(1){
time++;
if(time==1024*1024*256){
time=0;
////////////////////////////////////////////////////////////////////////////////
//read scale
fp_scale=fopen("scale.txt","r");  
if(fp_scale == NULL){
printf("no scale.txt");
}
else{
fgets(scale,100,fp_scale); 
fgets(offset,100,fp_scale); 
}
fclose(fp_scale);
//////////////////////////////////////////////////////////////////////////////// 
fp_r=fopen("data.txt","r");  
if(fp_r==NULL){
printf("no data.txt");
}
else{
len=GetFileLength(fp_r);   
printf("%d\n",len);       
////////////////////////////////////////////////////////////////////////////////
//read and write name
input=atoi(scale);
in_res=atoi(offset);
printf("%d\n",input);
printf("%d\n",in_res);
theshold=(int)in_res/1920;
printf("%d\n",theshold);   
printf("%d\n",in_res);  
printf("%s\n",s1);  
////////////////////////////////////////////////////////////////////////////////
if(theshold>len/1920)
{
printf("error");
while(1);
}
////////////////////////////////////////////////////////////////////////////////
for(i=0;i<10;i++){
if(resulation[i]==in_res){
printf("yes\n");
flag_yes=1;
break;
}
else{
printf("error2\n"); 
flag_yes=0;    
}
}
////////////////////////////////////////////////////////////////////////////////
if(flag_yes==1){
res=in_res/1920;
  for(t=0;t<theshold;t++){    
    for(i=0;i<in_res;i=i+1*res){
    j++;
    fseek(fp_r,1920*t+i+input,0);
    ccc=fgetc(fp_r)-48;
    data[j]=ccc*4;
    }
  }
}


    *Imagefinal=plot(data);         
    cvSaveImage("output.bmp", Imagefinal);	
    printf("done\n"); 
}system("rm data.txt scale.txt");
}//end while
}//end if time
    return EXIT_SUCCESS;
}
////////////////////////////////////////////////////////////////////////////////
unsigned long GetFileLength ( FILE * fileName)
{ 
unsigned long pos = ftell(fileName); 
unsigned long len = 0; 
fseek ( fileName, 0L, SEEK_END ); 
len = ftell ( fileName ); 
fseek ( fileName, pos, SEEK_SET ); 
return len; 
}
////////////////////////////////////////////////////////////////////////////////
IplImage   plot (double* data){
    IplImage *Image1;
    CvPoint FromPoint1,ToPoint1;
    CvPoint FromPoint2,ToPoint2;
    CvScalar Color;       
    int Thickness=1;   
    int Shift=0;        
    int i=0;
    int j=0;

    CvSize ImageSize1 = cvSize(1000,800);
    Image1 = cvCreateImage(ImageSize1,IPL_DEPTH_8U,3);
    Color=CV_RGB(255,0,0);


    //X
    for(i=0;i<100;i++){
    FromPoint1 = cvPoint(0+100*i,0);
    ToPoint1 = cvPoint(0+100*i,800);
    cvLine(Image1,FromPoint1,ToPoint1,Color,2*Thickness,CV_AA,Shift);
    }
    
    //Y    
    for(i=0;i<100;i++){
    FromPoint1 = cvPoint(0,0+100*i);
    ToPoint1 = cvPoint(1000,0+100*i);
    cvLine(Image1,FromPoint1,ToPoint1,Color,2*Thickness,CV_AA,Shift);
    }    
    
    //X
    for(i=0;i<100;i++){
    FromPoint1 = cvPoint(0+20*i,0);
    ToPoint1 = cvPoint(0+20*i,800);
    cvLine(Image1,FromPoint1,ToPoint1,Color,Thickness,CV_AA,Shift);
    }
    
    //Y    
    for(i=0;i<100;i++){
    FromPoint1 = cvPoint(0,0+20*i);
    ToPoint1 = cvPoint(1000,0+20*i);
    cvLine(Image1,FromPoint1,ToPoint1,Color,Thickness,CV_AA,Shift);
    }      
    
    //file_data
    Color=CV_RGB(255,255,255);
    for(i=0;i<1000;i=i+1){
    j++;
    FromPoint1 = cvPoint(j,800-data[i]);
    ToPoint1 = cvPoint(j+1,800-data[i+1]);
    cvLine(Image1,FromPoint1,ToPoint1,Color,Thickness,CV_AA,Shift);
    }    
    return *Image1;
}
