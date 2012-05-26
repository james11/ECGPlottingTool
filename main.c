#include <stdio.h>
#include <cv.h>
#include <highgui.h>

IplImage      plot (double* data); 
unsigned long GetFileLength ( FILE * fileName);




int main(int argc, char *argv[ ]) 
{
    IplImage *ECGImage;  
    CvSize ImageSize = cvSize(1000,800);
    ECGImage = cvCreateImage(ImageSize,IPL_DEPTH_8U,3);
    
    FILE *fp_scale;    
    FILE *fp_data;
    
    char offsetScale[100];  
    char resolutionScale[100];  
    
    int t=0;  
    int i=0;
    int j=0;
    
    double data[1000]={0};   
    
    
    
    int flag_yes=0;
    
    
    int resulationArray[10]={1920,1920*5,1920*10,1920*15,1920*30}; 
    int time=0;
    
    while(1){
        time++;
        if(time==1024*1024*256){ // Application run period
            time=0;
            
            ////////////////////////// Read scale.txt //////////////////////////////
            
            fp_scale=fopen("scale.txt","r");  
            if(fp_scale == NULL){
                printf("no scale.txt exist");
            }
            else{
                fgets(offsetScale,100,fp_scale); 
                fgets(resolutionScale,100,fp_scale); 
            }
            fclose(fp_scale);
            
            ////////////////////////// Read data.txt ////////////////////////////////
            
            fp_data=fopen("data.txt","r");  
            if(fp_data==NULL){
                printf("no data.txt exist");
            }
            else{
                unsigned long FileLength = GetFileLength(fp_data);   
                printf("File length = %lu\n",FileLength);       
                
                // Turn INFO of scale.txt into valuables
                int offset = atoi(offsetScale);
                int resolution = atoi(resolutionScale);
                printf("Offset scale = %d\n",offset);
                printf("Resolution scale = %d\n",resolution);
                
                int offsetSecond=offset/1920;
                int resolutionSecond=resolution/1920;
                printf("Offset time = %d seconds\n",offsetSecond);
                printf("Resolution time = %d seconds\n",resolutionSecond);
                
                
                
                if(resolutionSecond > FileLength/1920)
                {
                    printf("Out of file length");
                }
                
                for(i=0;i<10;i++){
                    if(resulationArray[i]==resolution){
                        printf("yes\n");
                        flag_yes=1;
                        break;
                    }
                    else{
                        printf("resolution not match\n"); 
                        flag_yes=0;    
                    }
                }
                
                /////////////////////////////////////////////////////////////////////////
                
                if(flag_yes==1){            
                    for(t = 0; t < resolutionSecond; t++){    
                        for(i = 0; i < 1000/resolutionSecond; i = i++){
                            fseek(fp_data, offset + 1920*t + 1920/(1000/resolutionSecond)*i,SEEK_SET);
                            int rawdata = fgetc(fp_data)-48;
                            data[j] = rawdata * 4;
                            j++;
                        }
                    }
                }
                
                *ECGImage=plot(data); 
                
                cvSaveImage("output.bmp", ECGImage,NULL);	
                printf("done\n"); 
                
            }
            
            system("rm data.txt scale.txt");
            
        }
    }
    return EXIT_SUCCESS;
}


////////////////////////// GetFileLength ////////////////////////////

unsigned long GetFileLength ( FILE * fileName)
{ 
    unsigned long pos = ftell(fileName); 
    unsigned long len = 0; 
    fseek ( fileName, 0L, SEEK_END ); 
    len = ftell ( fileName ); 
    fseek ( fileName, pos, SEEK_SET ); 
    return len; 
}


///////////////////////// plot ///////////////////////////////////////

IplImage   plot (double* data){
    IplImage *Image1;
    CvPoint FromPoint1,ToPoint1;
    //    CvPoint FromPoint2,ToPoint2;
    CvScalar Color=CV_RGB(255,0,0);  
    int Thickness=1;   
    int Shift=0;        
    int i=0;
    int j=0;
    
    CvSize ImageSize = cvSize(1000,800);
    Image1 = cvCreateImage(ImageSize,IPL_DEPTH_8U,3);
    
    
    
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
