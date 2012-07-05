#include <stdio.h>
#include <cv.h>
#include <highgui.h>

void          example (void); 
IplImage      plot (double* data, int secondInImage); 
unsigned long GetFileLength ( FILE * fileName);




int main(int argc, char *argv[ ]) 
{
    
    IplImage *Imagefinal;  
    CvSize ImageSize1 = cvSize(1000,800);
    Imagefinal = cvCreateImage(ImageSize1,IPL_DEPTH_8U,3);
    
    FILE *fp_scale;  
    FILE *fp_data;
    char offsetScale[100];  
    char resolutionScale[100]; 
    
    
    double data[1000]={0};   
    int flag_yes=0;
    
    
    
    int resolationArray[10]={1920,1920*5,1920*10,1920*15,1920*30}; 
    
    
    
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
                fgets(offsetScale,10,fp_scale); 
                fgets(resolutionScale,10,fp_scale); 
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
                
                int offsetSecond = (int)offset/1920;
                int resolutionSecond = (int)resolution/1920;
                printf("Offset time = %d seconds\n",offsetSecond);
                printf("Resolution time = %d seconds\n",resolutionSecond);
                
                
                if(offset + resolution > FileLength)
                {
                    printf("Out of file length");
                }
                
                ////////////////////////////////////////////////////////////////////////////////
                
                for(int i=0;i<10;i++){
                    if(resolationArray[i]==resolution){
                        printf("yes\n");
                        flag_yes=1;
                        break;
                    }
                    else{
                        printf("resolution not match\n"); 
                        flag_yes=0;    
                    }
                }
                
                
                ////////////////////////////////////////////////////////////////////////////////
                if(flag_yes==1){
                    int t = 0;
                    int i = 0;
                    int j = 0;
                    for(t=0;t<resolutionSecond;t++){    
                        for(i=0;i<1000/resolutionSecond;i++){
                            fseek(fp_data, 1920*t + 1920/(1000/resolutionSecond)*i + offset,SEEK_SET);
                            int rawData = fgetc(fp_data)-48;
                            data[j]=rawData * 4;
                            j++;
                        }
                    }
                }
                
                *Imagefinal=plot(data, resolutionSecond);         
                
                cvSaveImage("output.bmp", Imagefinal,NULL);	
                printf("done\n"); 
                
            }
            
            system("rm data.txt scale.txt");
            
        }
    }
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

///////////////////////////// Plotting Tool ////////////////////////////////

IplImage   plot (double* data, int secondInImage){
    IplImage *Image1;
    CvPoint FromPoint1,ToPoint1;
    
    int j=0;
    
    CvSize ImageSize1 = cvSize(1000,800);
    Image1 = cvCreateImage(ImageSize1,IPL_DEPTH_8U,3);
    CvScalar Color;       
    Color=CV_RGB(255,0,0);
    
    int largeThickness = 2;
    int smaleThickness = 1;
    int waveformThickness = 1;
    int largeGrid = (int)1000/(secondInImage * 5);
    int smaleGrid = (int)largeGrid/5;
    
    
    if(5 < secondInImage && secondInImage <= 15) largeThickness = smaleThickness;
    if(15 < secondInImage){
        largeThickness = largeThickness;
        largeGrid = largeGrid * 5;
    }
    
    // Vertical grid
    for(int i=0;i<1000;i++){
        if(i%largeGrid == 0){
            FromPoint1 = cvPoint(i,0);
            ToPoint1 = cvPoint(i,800);
            cvLine(Image1,FromPoint1,ToPoint1,Color,largeThickness,CV_AA,0);
        }
        else if (i%smaleGrid == 0 && secondInImage <= 5){
            FromPoint1 = cvPoint(i,0);
            ToPoint1 = cvPoint(i,800);
            cvLine(Image1,FromPoint1,ToPoint1,Color,smaleThickness,CV_AA,0);
        }
    }
    
    // Horizontal grid
    for(int i=0;i<800;i++){
        if(i%largeGrid == 0){
            FromPoint1 = cvPoint(0,i);
            ToPoint1 = cvPoint(1000,i);
            cvLine(Image1,FromPoint1,ToPoint1,Color,largeThickness,CV_AA,0);
        }
        else if(i%smaleGrid == 0 && secondInImage <= 5){
            FromPoint1 = cvPoint(0,i);
            ToPoint1 = cvPoint(1000,i);
            cvLine(Image1,FromPoint1,ToPoint1,Color,smaleThickness,CV_AA,0);
        }
    }
    
    
    // Waveform Plotting 
    Color=CV_RGB(255,255,255);
    for(int i=0;i<1000;i=i+1){
        j++;
        FromPoint1 = cvPoint(j,800-data[i]);
        ToPoint1 = cvPoint(j+1,800-data[i+1]);
        cvLine(Image1,FromPoint1,ToPoint1,Color,waveformThickness,CV_AA,0);
    }    
    return *Image1;
}
