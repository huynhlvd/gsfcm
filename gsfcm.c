
/**********************************************************/
/***** Read BMP with gsfcm operation **********/
/***** Version 1.1,  Jan. 24, 2009
/***** Implementations by Huynh Van Luong, Email: huynhlvd@gmail.com, *****/
/***** Embedded System Lab, University of Ulsan, Korea*****/
/***** Usage: gsfcm in_image number_of_segments termination_threshold ******/
/**********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define IMAX 100 
#define ROW		3
#define COL		3

double NB[ROW][COL]= {
	   {1.0,2.0,1.0},
	   {2.0,4.0,2.0},
	   {1.0,2.0,1.0},
   };

typedef struct {
	FILE *fp;
	char name [50];
} DISFILE;// Display output files

DISFILE *df;


double ex=0.000100, Vpc, Vpe, Vfs, Vxb;


int *v,*v1,*v2,*v3,*dis, *vt, *vt1, *vt2, *vt3; //
double **u, **w,**tu, **tw,**uw;//, 

int *x,*x1,*x2,*x3, sz=65536,cls=2, **tx; //
//double **u2; 
int *cluster, rows, cols; 
int **out,**out1, **out2, **out3; //

typedef struct {
   unsigned short int type;                 /* Magic identifier            */
   unsigned int size;                       /* File size in bytes          */
   unsigned short int reserved1, reserved2;
   unsigned int offset;                     /* Offset to image data, bytes */
}  HEADER;//__attribute__((__packed__));
// struct tagHEADER ;

typedef struct {
   unsigned int size;               /* Header size in bytes      */
   int width,height;                /* Width and height of image */
   unsigned short int planes;       /* Number of colour planes   */
   unsigned short int bits;         /* Bits per pixel            */
   unsigned int compression;        /* Compression type          */
   unsigned int imagesize;          /* Image size in bytes       */
   int xresolution,yresolution;     /* Pixels per meter          */
   unsigned int ncolours;           /* Number of colours         */
   unsigned int importantcolours;   /* Important colours         */
}  INFOHEADER;//__attribute__((__packed__));
// struct tagINFOHEADER ;

typedef struct {                     /**** Colormap entry structure ****/
    unsigned char  b,g,r, junk;			 /* Blue value, Green value, Red value, Reserved value */
} COLOURINDEX;

HEADER header;
INFOHEADER infoheader;
COLOURINDEX  colourindex[256]; /* Image colormap */

typedef struct {
  
	unsigned char blue;
	unsigned char green;
	unsigned char red;
} Color;
/*
 *
 *  BMP functions 
 *
 */

void display(int k,FILE *fp1,int xsize,int ysize); //
void init();
void step2(); //
void step3();
void step4();

void display_rgb(int k,FILE *fp1,int xsize,int ysize); //
void init_rgb();
void step2_rgb(); //
void step3_rgb();
void step4_rgb();

int main (int argc, char **argv)
{
   int i, j;

   unsigned char *pb;//pointer to gray image
   FILE *fi, *fo;

   Color *pcolor;// pointer to color components

/***************Print Help to enter parameters***************************/
  if (argc < 2) {
    printf("Usage:  fccm <options>\n");
    printf("Please type -help for instructions\n");
    exit(-1);
  }
  /* parse the command line arguments */
 /***** Usage: fccm in_image number_of_segments termination_threshold******/
  for (i = 0; i < argc; i++)
    if (!strcmp(argv[i], "-help")) {
      printf("\tfccm in_image number_of_segments termination_threshold\n");
      printf("\t  in_image             : Set input bitmap file (8 0r 24 bits per pixel)\n");
      printf("\t  number_of_segments   : Set number of output segmented images\n");
      printf("\t  termination_threshold: Set termination threshold for stopping the algorithm\n");
      exit(0);
    } 

  if (argv[2]!=NULL) cls=atoi(argv[2]);
  else 
  {
	printf("Please enter number of output segmented images\n");
    exit(0);
  }

  if (argv[3]!=NULL) ex=atof(argv[3]);
  else
  {
	printf("Please enter termination threshold for stopping the algorithm\n");
    exit(0);
  }

/************************************************************************/
/*     Read bitmap file                                                 */
/************************************************************************/
  if ((fi = fopen(argv[1],"rb")) == NULL) { 
    printf("File open error !!\n");
    return 1;
  }
  //fi is read from input file
  fread (&header, sizeof(header)-2, 1, fi);
  fread (&infoheader, sizeof (infoheader), 1, fi);
  fread (&colourindex, 256*sizeof (COLOURINDEX), 1, fi);
  rows=infoheader.height;
  cols=infoheader.width;
  sz=rows*cols*infoheader.bits/8;


  pcolor = (Color*)malloc(rows*cols*sizeof(Color)); 
  pb= (unsigned char*)malloc(rows*cols*sizeof(unsigned char));

  df = (DISFILE*)malloc(cls*sizeof(DISFILE));





  fo=fopen("out.bmp", "wb");
  fwrite(&header,sizeof(header)-2,1,fo);
  fwrite(&infoheader,sizeof(infoheader),1,fo);    
  fwrite(&colourindex,256*sizeof(COLOURINDEX),1,fo);

/*****************************************************/
/***************** Finish reading file ***************/
/*****************************************************/
//printf ("%i\n%i\n%i\n%i\n%i\n",sizeof(pb), rows, cols, sizeof(pcolor),infoheader.bits);
/**********************************************************/
  switch (infoheader.bits) {
	case 1:
		break;
	case 4:
		break;
/*******************************************************************************************/
//Case of gray image
/*******************************************************************************************/

    case 8:
		fread(pb,sizeof(char),sz,fi);
		x = (int*)malloc(sz*sizeof(int));
		tx =(int**)malloc(rows*sizeof(int*));
		   for (i=0; i<rows; i++)
			  tx[i] = (int*)malloc(cols*sizeof(int));

		cluster = (int*)malloc(sz*sizeof(int));
		v = (int*)malloc(cls*sizeof(int));
		vt = (int*)malloc(cls*sizeof(int));
	    dis = (int*)malloc(cls*sizeof(int));
		u =(double**)malloc(sz*sizeof(double*));
		   for (i=0; i<sz; i++)
			  u[i] = (double*)malloc(cls*sizeof(double));
		tu =(double**)malloc(rows*sizeof(double*));
		   for (i=0; i<rows; i++)
			  tu[i] = (double*)malloc(cols*sizeof(double));
		tw =(double**)malloc(rows*sizeof(double*));
		   for (i=0; i<rows; i++)
			  tw[i] = (double*)malloc(cols*sizeof(double));
		w =(double**)malloc(sz*sizeof(double*));
		   for (i=0; i<sz; i++)
			  w[i] = (double*)malloc(cls*sizeof(double));
		uw =(double**)malloc(sz*sizeof(double*));
		   for (i=0; i<sz; i++)
			  uw[i] = (double*)malloc(cls*sizeof(double));
		out =(int**)malloc(cls*sizeof(int*));
		  for (i=0; i<cls; i++)
 			  out[i] = (int*)malloc(sz*sizeof(int));

/**********************************************************/
  for (i=0; i<rows; i++) 
	  for (j=0; j<cols; j++)
	  {
			x[i*cols+j] = pb[i*cols+j];
			tx[i][j] = pb[i*cols+j];
	  }
  fwrite(pb,sizeof(char),sz,fo);
/**********************************************************/

  init(); //
  step2(); //
  step3(); //
  step4(); //
  //display 
  for (i=0; i<cls; i++)
  {
	 sprintf (df[i].name, "%s_%d%s", "out",i+1,".bmp");
	 display(i,df[i].fp,rows,cols);
  }
  free(x);
  free(cluster);
  free(v);
  free(dis);
  free(u);
  free(vt);
  free(out);
/**********************************************************/
            break;
/*******************************************************************************************/
////////case of color image (24bits)
/*******************************************************************************************/

       case 24:
	  fread(pcolor,sizeof(char),sz,fi);
	  fwrite(pcolor,sizeof(char),sz,fo);
	  sz=sz/3;
	  //printf("%d\t", sz);

	  x1 = (int*)malloc(sz*sizeof(int));
	  x2 = (int*)malloc(sz*sizeof(int));
	  x3 = (int*)malloc(sz*sizeof(int));
	  cluster = (int*)malloc(sz*sizeof(int));
	  v1 = (int*)malloc(cls*sizeof(int));
	  v2 = (int*)malloc(cls*sizeof(int));
	  v3 = (int*)malloc(cls*sizeof(int));

	  vt1 = (int*)malloc(cls*sizeof(int));
	  vt2 = (int*)malloc(cls*sizeof(int));
	  vt3 = (int*)malloc(cls*sizeof(int));

	  dis = (int*)malloc(cls*sizeof(int));
	  u =(double**)malloc(sz*sizeof(double*));
		  for (i=0; i<sz; i++)
			  u[i] = (double*)malloc(cls*sizeof(double));
	  w =(double**)malloc(sz*sizeof(double*));
		   for (i=0; i<sz; i++)
			  w[i] = (double*)malloc(cls*sizeof(double));

	  out1 =(int**)malloc(cls*sizeof(int*));
		  for (i=0; i<cls; i++)
 			  out1[i] = (int*)malloc(sz*sizeof(int));
	  out2 =(int**)malloc(cls*sizeof(int*));
		  for (i=0; i<cls; i++)
 			  out2[i] = (int*)malloc(sz*sizeof(int));
	  out3 =(int**)malloc(cls*sizeof(int*));
		  for (i=0; i<cls; i++)
 			  out3[i] = (int*)malloc(sz*sizeof(int));

//processing with blue color
/**********************************************************/
  for (i=0; i<rows; i++) 
	  for (j=0; j<cols; j++) {
			x1[i*cols+j]= pcolor[i*cols+j].red;					
			x2[i*cols+j]= pcolor[i*cols+j].green;	
			x3[i*cols+j]= pcolor[i*cols+j].blue;
	  }


/**********************************************************/
 
  init_rgb(); //
  step2_rgb(); //
  step3_rgb(); //
  step4_rgb(); //
  //display 
  //display 
  for (i=0; i<cls; i++)
  {
	 sprintf (df[i].name, "%s_%d%s", "out",i+1,".bmp");
	 display_rgb(i,df[i].fp,rows,cols);
  }

  free(x1);
  free(x2);
  free(x3);
  free(cluster);

  free(v1);
  free(v2);
  free(v3);
  free(dis);
  free(u);


    free(out1);
  free(out2);
  free(out3);
/**********************************************************/
            break;
         } 
/**********************************************************/

/* Write an output bitmap file*****************************/
/**********************************************************/
 
  free(pcolor);
  free(pb);
  fclose (fi);
  fclose (fo);
/*********************************************/
  return 0;
}

/**********     Initiation   ********************/
void init()
{
   int i,j;
   for(i = 0 ; i < sz ; i++)
   {
  	   for(j=0 ; j < cls ;  j++)
	   {
		  if((i % cls) == j)
            u[i][j] = 1.000000;
		  else
			u[i][j] = 0.000000;
		  out[j][i] = x[i];		 
	   }   	  
   }	   
   for(i=1;i<(cls+1);i++)
   {    
	   v[i-1] = (i* 255 )/ cls ;
   }  
}
/*************  End Initiation   ****************/
/*---------------------------------------------------------------------------*/
/*************  Computing new weights of each pixel and new centers of each cluster **********/
void step2()
{
  int i,j, m, n, k;
  double sum=0.000000, sumtx=16.0;
  //int D=0;
  double denoi =0.000000;
  double numer =0.000000;

  for(j=0 ; j < cls ;  j++)
  {
	   vt[j] = v[j];
  }


  // compute new weights
  for(i = 0 ; i < sz ; i++)
  {
	  sum = 0.000000;
	  for(j= 0 ; j < cls ; j++)
	  {       
		  dis[j] = ((x[i]-v[j]) * (x[i]-v[j]));
		  if(dis[j] != 0 )
		  sum = sum + 1/(double)dis[j];	  
	  }
	  for(j=0 ; j < cls ; j++)
	  {
	      if(dis[j] != 0 )
		  {
			u[i][j] = (1/(double)dis[j]) / (double)sum;
			dis[j]=0;
		  }
		  else
			u[i][j] = 1.000000;
	  }
  }
  //Compute new spatial weights
/**********************************************************/
  for (k=0; k<cls; k++)	
  {
	  for (i=0; i<rows; i++) 
		  for (j=0; j<cols; j++)
				// Compute subfunction new tu=g(u);
				tu[i][j]= pow(u[i*cols+j][k],1);// tu=u

	  for (i=0; i<rows; i++) 
		  for (j=0; j<cols; j++) 
		  {
			if ((i>=((ROW-1)/2))&&(j>=((COL-1)/2))&&(i<(rows-(ROW-1)/2))&&(j<(cols-(COL-1)/2))) 
			{
			  sumtx=0.000000;
			  for (m=0; m<ROW; m++)
				for (n=0; n<COL; n++) 
				{
					if (tx[i][j]==tx[i+m-(ROW-1)/2][j+n-(COL-1)/2])
						NB[m][n]=1;
					else
					{
						NB[m][n]=1/((tx[i][j]-tx[i+m-(ROW-1)/2][j+n-(COL-1)/2])*(tx[i][j]-tx[i+m-(ROW-1)/2][j+n-(COL-1)/2]));
						
					}
					sumtx=sumtx+NB[m][n];
				}
			}
			if ((i<((ROW-1)/2))||(j<((COL-1)/2))||(i>=(rows-(ROW-1)/2))||(j>=(cols-(COL-1)/2))) 
			{
				tw[i][j]=tu[i][j];
			}
			else
			{
				for (m=0; m<ROW; m++)
					for (n=0; n<COL; n++) 
						sum = sum + tu[i+m-(ROW-1)/2][j+n-(COL-1)/2]*NB[m][n];
				tw[i][j]= sum/sumtx;
				sum=0.000000;
				sumtx=0.000000;
			}
		  }

	  for (i=0; i<rows; i++) 
		  for (j=0; j<cols; j++)
		  {
				w[i*cols+j][k]=pow(tw[i][j],1); // Compute subfunction new w=f(w);
		  }
  }

/**********************************************************/
  // compute new combined weights

  for(i = 0 ; i < sz ; i++)
  {
	  for(j= 0 ; j < cls ; j++)
	  {
          //numer =  numer + ((w[i][j] * w[i][j]) * (u[i]));
          denoi =  denoi + (u[i][j])*(w[i][j]);
	  }
	  for (j=0; j<cls; j++)
	  {
	      uw[i][j] = ((u[i][j])*(w[i][j]))/denoi;	    
	  }
	  denoi = 0.000000;
  } 
/**********************************************************/
  // compute new centers
  for(j = 0 ; j < cls ; j++)
  {
	  for(i= 0 ; i < sz ; i++)
	  {
          numer =  numer + ((uw[i][j] * uw[i][j]) * (x[i]));
          denoi =  denoi + (uw[i][j] * uw[i][j]);
	  }
	  v[j] = (int)(numer / denoi);
	  numer = 0.000000;
	  denoi = 0.000000;	    
  } 
}
/************************* End Computing new weights and new centers ***********************/
/*-----------------------------------------------------------------------------------------*/
/******************** Checking termination condition ********************/
void step3()
{
	int i,n; 
	double maxdiff=0.000000;
	double max;
	for(n=0; n<IMAX; n++)
	{
		maxdiff=0.000000;

			for(i=0 ; i < cls ;  i++)
			{
				max = (double)(abs(vt[i] - v[i])/(double)v[i]);
				if(max > maxdiff)
				{
					maxdiff = max ;
				}        
			}			
			//printf("%f-\t",max);

		printf("%d*%f*\t",n,maxdiff);

		if(maxdiff < ex)
		{
			return;
		}  
		else
		{

			step2();		
		}
	}
}
/*********** End if satisfying termination condition *******************/
/*---------------------------------------------------------------------*/
/*********** Assign value of clusters according to the max weights *************/
void step4()
{
	int i,j;
	double maxWeight=0.000000;
	int kmax, mincls=65025, av=0;
	//Compute average value of v[i]
    for (i=0; i<cls; i++)
		av=av+v[i];
	av=av/cls;
	/////////////
	for(i = 0 ; i < sz ; i++)
	{
		maxWeight = 0.000000;
		//Compute performance parameters
		for(j=0 ; j < cls ;  j++)
		{
			Vpc=Vpc + uw[i][j]*uw[i][j];
			Vpe=Vpe + uw[i][j]*log10(uw[i][j]);
			Vfs=Vfs + uw[i][j]*uw[i][j]*((x[i]-v[j])*(x[i]-v[j])-(v[j]-av)*(v[j]-av));//()-()
			Vxb=Vxb + uw[i][j]*uw[i][j]*(x[i]-v[j])*(x[i]-v[j]);
			//Find max weight and assign cluster number
			if(maxWeight < uw[i][j])
			{
				maxWeight = uw[i][j];
				kmax = j;
			}        
		}
		cluster[i] = kmax;
		///////////////////
	}
	//Finding min value of between clusters
	for (i=0; i<cls; i++)
	{
		for (j=0; j<cls; j++)
		{
			if ((((v[i]-v[j])*(v[i]-v[j]))!=0)&&(((v[i]-v[j])*(v[i]-v[j]))<mincls))
				mincls=(v[i]-v[j])*(v[i]-v[j]);
		}
		printf("\nV[%d]=%d,\t",i,v[i]);
	}
	////////////////////
	Vpc=Vpc/(sz);
	Vpe=-(Vpe/(sz));
	Vfs=Vfs/1000000;
	Vxb=Vxb/(sz*mincls);
	printf("\n\nVpc=%f%\nVpe=%f%\nVfs=%f%[10^6]\nVxb=%f%\nmincls=%d%\naveragecls=%d\n",Vpc,Vpe,Vfs,Vxb,mincls,av);
	printf("\n\n%f%\t%f%\t%f%\t%f%\t\n",Vpc,Vpe,Vfs,Vxb);
}
/***************** End assigning clusters ***************************************/
/*-------------------------------------------------------------------------------*/
/***************** Output segmented images ***************************************/
void display(int k,FILE *f,int xsize,int ysize)
{
	int i;
	unsigned char *p;

	p = (unsigned char*)malloc(xsize*ysize*sizeof(unsigned char));
	for(i=0;i<sz;i++)
	{
		if(cluster[i] != k)
			out[k][i] = 0;
		p[i] = (unsigned char)out[k][i];
	}
	if((f=fopen(df[k].name,"wb")) == NULL)
	{
		printf("error");
	}
   
	fwrite(&header,sizeof(header)-2,1,f);
	fwrite(&infoheader,sizeof(infoheader),1,f);    
	fwrite(&colourindex,256*sizeof(COLOURINDEX),1,f); 

	fwrite(p, sizeof(char), xsize*ysize, f);
	fclose(f);
}
/*********************** End Output images *********************************/
/*
/*						RGB Case
/*
/**********************************************************************************************************************************/
/**********     Initiation   ********************/
void init_rgb()
{
   int i,j;
   for(i = 0 ; i < sz ; i++)
   {
  	   for(j=0 ; j < cls ;  j++)
	   {
		  if((i % cls) == j)
            u[i][j] = 1.000000;
		  else
			u[i][j] = 0.000000;
		  out1[j][i] = x1[i];		 
		  out2[j][i] = x2[i];	
		  out3[j][i] = x3[i];	

	   }   	  
   }	   
   for(i=1;i<(cls+1);i++)
   {    
	   v1[i-1] = (255*i ) / cls ;
	   v2[i-1] = (255*i ) / cls ;
	   v3[i-1] = (255*i ) / cls ;
   }  
}
/*************  End Initiation   ****************/
/*---------------------------------------------------------------------------*/
/*************  Computing new weights of each pixel and new centers of each cluster **********/
void step2_rgb()
{
  int i,j, m, n;
  double sum=0.000000;
  //int D=0;
  double denoi =0.000000;
  double numer1 =0.000000;
  double numer2 =0.000000;
  double numer3 =0.000000;

  for(j=0 ; j < cls ;  j++)
  {
	   vt1[j] = v1[j];
	   vt2[j] = v2[j];
	   vt3[j] = v3[j];
  }

  // compute new weights
  for(i = 0 ; i < sz ; i++)
  {
	  for(j= 0 ; j < cls ; j++)
	  {       
		  dis[j] = ((x1[i]-v1[j]) * (x1[i]-v1[j]))+((x2[i]-v2[j]) * (x2[i]-v2[j]))+((x3[i]-v3[j]) * (x3[i]-v3[j]));
		  if(dis[j] != 0 )
		  sum = sum + ((double)1/(double)dis[j]);	  
	  }
	  for(j=0 ; j < cls ; j++)
	  {
	      if(dis[j] != 0 )
		  {
			u[i][j] = ((double)1/(double)dis[j]) / (double)sum;
			dis[j]=0;
		  }
		  else
			u[i][j] = 1.000000;
	  }
	  sum=0.000000;
  }
  /**********************************************************/
  for (i=0; i<sz; i++) 
	  for (j=0; j<cls; j++) 
	  {
		if ((i<((ROW-1)/2))||(j<((COL-1)/2))||(i>=(sz-(ROW-1)/2))||(j>=(cls-(COL-1)/2))) 
		{
			w[i][j]=u[i][j];
		}
		else
		{
			for (m=0; m<ROW; m++)
				for (n=0; n<COL; n++) 
					sum = sum + u[i+m-(ROW-1)/2][j+n-(COL-1)/2]*NB[m][n];
			w[i][j]= sum;
			sum=0.000000;
		}
	  }
/**********************************************************/

  // compute new centers
  for(j = 0 ; j < cls ; j++)
  {
	  for(i= 0 ; i < sz ; i++)
	  {
          numer1 =  numer1 + ((w[i][j] * w[i][j]) * (x1[i]));
          numer2 =  numer2 + ((w[i][j] * w[i][j]) * (x2[i]));
          numer3 =  numer3 + ((w[i][j] * w[i][j]) * (x3[i]));
          denoi =  denoi + (w[i][j] * w[i][j]);
	  }
	  v1[j] = (int)(numer1 / denoi);
	  v2[j] = (int)(numer2 / denoi);
	  v3[j] = (int)(numer3 /denoi);

	  numer1 = 0.000000;
	  numer2 = 0.000000;
	  numer3 = 0.000000;

	  denoi = 0.000000;	    
  } 
}
/************************* End Computing new weights and new centers ***********************/
/*-----------------------------------------------------------------------------------------*/
/******************** Checking termination condition ********************/
void step3_rgb()
{
	int i,n; 
	double maxdiff=0.000000;
	double max;
	printf("%f\t",ex);
	for(n=0; n<IMAX; n++)
	{
		maxdiff=0.000000;

		for(i=0 ; i < cls ;  i++)
		{
			max = (double)(abs(vt1[i] - v1[i])+abs(vt2[i] - v2[i])+abs(vt3[i] - v3[i]))/(double)(v1[i]+v2[i]+v3[i]);
				
		    if(max > maxdiff)
			{
			    maxdiff = max ;
			}        
		}

		printf("*");
		if(maxdiff < ex)
		{
			return;
		}  
		else
			step2_rgb();
	}
}
/*********** End if satisfying termination condition *******************/
/*---------------------------------------------------------------------*/
/*********** Assign value of clusters according to the max weights *************/
void step4_rgb()
{
	int i,j;
	double maxWeight=0.000000;
	int kmax;
	for(i = 0 ; i < sz ; i++)
	{
		maxWeight = 0.000000;
		for(j=0 ; j < cls ;  j++)
		{
			if(maxWeight < w[i][j])
			{
				maxWeight = w[i][j];
				kmax = j;
			}        
		}
		cluster[i] = kmax;
	}
}
/***************** End assigning clusters ***************************************/
/*-------------------------------------------------------------------------------*/
/***************** Output segmented images ***************************************/
void display_rgb(int k,FILE *f,int xsize,int ysize)
{
	int i;
	Color *p_rgb;// pointer to color components
	p_rgb = (Color*)malloc(xsize*ysize*sizeof(Color));

	for(i=0;i<sz;i++)
	{
		if(cluster[i] != k)
		{
			out1[k][i] = 0;
			out2[k][i] = 0;
			out3[k][i] = 0;
		}
		p_rgb[i].red = (unsigned char)out1[k][i];
		p_rgb[i].green = (unsigned char)out2[k][i];
		p_rgb[i].blue = (unsigned char)out3[k][i];

	}
	if((f=fopen(df[k].name,"wb")) == NULL)
	{
		printf("error");
	}
   
	fwrite(&header,sizeof(header)-2,1,f);
	fwrite(&infoheader,sizeof(infoheader),1,f);    
	fwrite(&colourindex,256*sizeof(COLOURINDEX),1,f); 

	fwrite(p_rgb, sizeof(char), xsize*ysize*sizeof(Color), f);
	fclose(f);
}
/*********************** End Output images *********************************/