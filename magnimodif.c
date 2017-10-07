#include<stdio.h>
#include<math.h>
#include<stdlib.h>


long int IntensitySum(int vert,int hori,int boxs,int hsimage,double* MOI,double* Map){

    int i,j,k;

        for(j=0;j<(2*boxs+1);++j)
            {
            for(i=0;i<(2*boxs+1);++i)
                {
                MOI[i+j*(2*boxs+1)]=Map[vert-boxs+i+(hori-boxs+j)*hsimage];
                }
            }

    long int sum;
    sum=0;

        for(k=0;k<(2*boxs+1)*(2*boxs+1);++k)
            {
            sum+=MOI[k];
            }

    return sum;
}

double absd(double szam){

    double ki;

        if(szam<0){
            ki=(-1)*szam;
        }

        else{
            ki=szam;
        }

    return ki;
}

double maxdev(double mavg,double mv1,double mv2,double mv3){

    double dev,temp;

    dev=absd(mavg-mv1);

    temp=absd(mavg-mv2);

        if(dev<temp){
            dev=temp;
        }

    temp=absd(mavg-mv3);

        if(dev<temp){
            dev=temp;
        }

    return dev;
}


//******************************************************************************************************************************

int main(int argc,char* argv[]){      //First the name of the file, then the coordinates of the stars: 3 compare stars and 1 which I want to measure
                                    //We need the indexes of the stars and the box sizes, and we need the size of the image as well

    FILE *file;
    file=fopen(argv[1],"r");
    if ( file == 0 ){
            fprintf(stderr, "ERR=Could not open file\n" );
            exit(1);
        }

    int vsimage,hsimage;            //Vertical and horizontal size of image
    vsimage=atoi(argv[2]);
    hsimage=atoi(argv[3]);

    double* Map;
    Map=(double*)malloc(vsimage*hsimage*sizeof(double));    //Size of the image


    int i,j;
        for(i=0;i<vsimage;i++)
            {
            for(j=0;j<hsimage;j++)
                {
                fscanf(file,"%lf",&Map[i*hsimage+j]);
                }
            }

    fclose(file);

    int NOC; // Number of Comparsion stars
    NOC=atoi(argv[4]);

    int* verti; int* hori;
    verti=(int*)malloc(NOC*sizeof(int));
    hori=(int*)malloc(NOC*sizeof(int));

    for(i=0;i<NOC;i++){
        verti[i]=atoi(argv[2*i+5]);
        hori[i]=atoi(argv[2*i+6]);
    }

    int hb,vb;
    vb=atoi(argv[2*NOC+5]);
    hb=atoi(argv[2*NOC+6]);


//    int v1,h1,v2,h2,v3,h3,hb,vb;          //Vertical and horizontal coordinates of the compare stars
//    v1=atoi(argv[4]);             //-1, because when you look at an intensity map, the indexes will start from 1 on the sides, but here the starting index is 0
//    h1=atoi(argv[5]);
//    v2=atoi(argv[6]);
//    h2=atoi(argv[7]);
//    v3=atoi(argv[8]);
//    h3=atoi(argv[9]);


    int boxs;                       //From the middle pixel to the edge of the box
    boxs=atoi(argv[2*NOC+7]);

    int vvari,hvari;                //Variable's coordinates
    vvari=atoi(argv[2*NOC+8]);
    hvari=atoi(argv[2*NOC+9]);

//    printf("%d %d %d %d %d %d %d %d %d\n",h1,v1,h2,v2,h3,v3,boxs,hvari,vvari);

    double* Comp;// double* Comp2; double* Comp3;
    double* Vari; double* Backg;                           //The matrices we will work with

    //Comp2=(double*)malloc((2*boxs+1)*(2*boxs+1)*sizeof(double));
    //Comp3=(double*)malloc((2*boxs+1)*(2*boxs+1)*sizeof(double));
    Vari=(double*)malloc((2*boxs+1)*(2*boxs+1)*sizeof(double));
    Backg=(double*)malloc((2*boxs+1)*(2*boxs+1)*sizeof(double));


    long int* SumComp;
    SumComp=(long int*)malloc(NOC*sizeof(long int));
    long int SumVari,SumBack;//SumComp1,SumComp2,SumComp3;                //Now we have the intensity sums and the matrices containing the stars

    for(i=0;i<NOC;i++){
        Comp=(double*)malloc((2*boxs+1)*(2*boxs+1)*sizeof(double));
        SumComp[i]=IntensitySum(hori[i],verti[i],boxs,hsimage,Comp,Map);

  //      for(i=0;i<2*boxs+1;i++)
  //          {
  //          for(j=0;j<2*boxs+1;j++)
  //              {
  //              printf("%14f",Comp[i*(2*boxs+1)+j]);
  //              }
  //          printf("\n");
  //          }
  //      printf("\n");

        free(Comp);
    }

    //SumComp2=IntensitySum(h2,v2,boxs,hsimage,Comp2,Map);
    //SumComp3=IntensitySum(h3,v3,boxs,hsimage,Comp3,Map);
    SumVari=IntensitySum(hvari,vvari,boxs,hsimage,Vari,Map);
    SumBack=IntensitySum(hb,vb,boxs,hsimage,Backg,Map);

    for(i=0;i<NOC;i++){

        if(SumBack>SumComp[i]){

                fprintf(stderr,"Background sum is higher than at least one of the star's");
                exit(1);

        }
    }


    if(SumBack>SumVari){

        fprintf(stderr,"Background sum is higher than at least one of the star's");
        exit(1);
    }





  //      for(i=0;i<2*boxs+1;i++)
  //          {
  //          for(j=0;j<2*boxs+1;j++)
  //              {
  //              printf("%14f",Comp1[i*(2*boxs+1)+j]);
  //              }
  //          printf("\n");
  //          }
  //      printf("\n");

  //      for(i=0;i<2*boxs+1;i++)
  //          {
  //          for(j=0;j<2*boxs+1;j++)
  //              {
  //              printf("%14f",Comp2[i*(2*boxs+1)+j]);
  //              }
  //          printf("\n");
  //          }
  //      printf("\n");

  //      for(i=0;i<2*boxs+1;i++)
  //          {
  //          for(j=0;j<2*boxs+1;j++)
  //              {
  //              printf("%14f",Comp3[i*(2*boxs+1)+j]);
  //              }
  //          printf("\n");
  //          }
  //      printf("\n");

  //      for(i=0;i<2*boxs+1;i++)
  //          {
  //          for(j=0;j<2*boxs+1;j++)
  //              {
  //              printf("%14f",Vari[i*(2*boxs+1)+j]);
  //              }
  //          printf("\n");
  //          }
  //      printf("\n");


  for(i=0;i<NOC;i++){
    printf("%ld ",SumComp[i]);
  }

  printf("%ld %ld\n",SumVari,SumBack);

//    printf("%ld %ld %ld %ld %ld\n",SumComp1,SumComp2,SumComp3,SumVari,SumBack);

free(Map);
free(Vari);
free(Backg);




    //Now we will have to add the know magnitudes of the compare stars, so that we can calculate the average brightness of the background, which we have to subtract.
    //We will get three values, we will take their average

    double mc1,mc2,mc3;
    mc1=atof(argv[2*NOC+10]);
    mc2=atof(argv[2*NOC+11]);
    mc3=atof(argv[2*NOC+12]);

   // double b1,b2,b3;        //These are the average background brightnesses, but pay ATTENTION, these are the sums for the whole area, not the background brightness of one pixel
  //  b1=(pow(10.0,-0.4*(mc2-mc1))*(double)(SumComp2)-(double)(SumComp1))/(pow(10.0,-0.4*(mc2-mc1))-1)/(pow((2*boxs+1),2));
  //  b2=(pow(10.0,-0.4*(mc3-mc1))*(double)(SumComp3)-(double)(SumComp1))/(pow(10.0,-0.4*(mc3-mc1))-1)/(pow((2*boxs+1),2));
  //  b3=(pow(10.0,-0.4*(mc2-mc3))*(double)(SumComp2)-(double)(SumComp3))/(pow(10.0,-0.4*(mc2-mc3))-1)/(pow((2*boxs+1),2));


    double mv1,mv2,mv3;
    mv1=mc1-2.5*log10(((double)(SumVari)-(double)(SumBack))/((double)(SumComp1)-(double)(SumBack)));
    mv2=mc2-2.5*log10(((double)(SumVari)-(double)(SumBack))/((double)(SumComp2)-(double)(SumBack)));
    mv3=mc3-2.5*log10(((double)(SumVari)-(double)(SumBack))/((double)(SumComp3)-(double)(SumBack)));

    printf("%lf %lf %lf\n",mv1,mv2,mv3);

    double mavg;
    mavg=(mv1+mv2+mv3)/3;

    double relerr;
    relerr=maxdev(mavg,mv1,mv2,mv3)/mavg;


//If we want to write the values to a file

    FILE *output;
    output=fopen(argv[2*NOC+13],"r");

    if ( output == 0 ){
           printf("%lf\n %lf\n",mavg,relerr);
           exit(1);
        }
    else{

        int sorb,m;
        double* buffer;
        buffer=(double*)malloc(2*sizeof(double));

        i=0;
        sorb=1;
            while(fscanf(output,"%lf",&buffer[i])==1){
                m=2*sorb-1;

                if(i==m){
                    sorb+=1;
                    buffer=(double*)realloc(buffer,2*sorb*sizeof(double));
                }
                i+=1;
        }
        fclose(output);

  //      for(m=0;m<i;m++){
  //          printf("%lf  ",buffer[m]);
  //      }

        output=fopen(argv[2*NOC+13],"w");

            j=0;
            while(j<i){
                fprintf(output,"%10f %10f\n",buffer[j],buffer[j+1]);
                j+=2;
            }

                fprintf(output,"%10f %10f\n",mavg,relerr);

        fclose(output);

    }


return 0;
}
