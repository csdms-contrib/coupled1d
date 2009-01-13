#ifdef HAVE_MALLOC_H
# include<malloc.h>
#endif
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#define FREE_ARG char*
#define NR_END 1

float *vector(nl,nh)
long nh,nl;
/* allocate a float vector with subscript range v[nl..nh] */
{
        float *v;

        v=(float *)malloc((unsigned int) ((nh-nl+1+NR_END)*sizeof(float)));
        return v-nl+NR_END;
}

int *ivector(nl,nh)
long nh,nl;
/* allocate an int vector with subscript range v[nl..nh] */
{
        int *v;

        v=(int *)malloc((unsigned int) ((nh-nl+1+NR_END)*sizeof(int)));
        return v-nl+NR_END;
}

main()
{    FILE *fp1,*fp2;
     float transport,c,D,*hold,time,factor,max,totsed,deltah,*h,sum;
     int bedrock,xc,i,lattice_size,check;

     fp1=fopen("appalachiansspace1","w");
     fp2=fopen("appalachianstime1","w");
     c=0.1; 
     D=1.0;
     lattice_size=128; /* delta= 1 km, so basin is 128 km in length */
     bedrock=16;       /* bedrock-alluvial transition 16 km from divide */
     h=vector(1,lattice_size);
     hold=vector(1,lattice_size);
     xc=bedrock;
     for (i=1;i<=lattice_size;i++)
      if (i>bedrock) 
       {h[i]=0.0;
        hold[i]=0.0;
        /* plot the initial condition first */ 
        fprintf(fp1,"%f %f\n",i/(float)(lattice_size),h[i]);}
         else  {h[i]=1.0;
                hold[i]=1.0;
                fprintf(fp1,"%f %f\n",i/(float)(lattice_size),h[i]);}     
     factor=1.0;
     time=0.0;
     check=10000;
     while (time<100000.0) 
      {if (time>check)  
        {sum=0;
         check+=10000;
         for (i=1;i<=lattice_size;i++)
          {sum+=h[i];
           fprintf(fp1,"%f %f\n",i/(float)(lattice_size),h[i]);}
         fprintf(fp2,"%f %f\n",time,sum/lattice_size);
         fprintf(fp1,"%f %f\n",xc/(float)(lattice_size),h[xc]);}
       max=0;
       totsed=0;
       deltah=c*factor*(1/(float)(lattice_size))*(h[1]-h[2]);
       if (fabs(deltah)>max) max=fabs(deltah);
       h[1]-=deltah;
       totsed=deltah;
       for (i=2;i<=lattice_size-1;i++)
        {deltah=c*factor*(i/(float)(lattice_size))*(hold[i]-hold[i+1]);
         totsed+=deltah;
         transport=D*factor*(i/(float)(lattice_size))*(hold[i]-hold[i+1]);
         if ((transport>(totsed+0.01))&&(i<=bedrock))
          {xc=i;
           if (fabs(deltah)>max) max=fabs(deltah);
           h[i]-=deltah;}
         else  
          {deltah=D*factor*((i-1)/(float)(lattice_size))*(hold[i-1]-hold[i])-
            D*factor*(i/(float)(lattice_size))*(hold[i]-hold[i+1]);
           if (fabs(deltah)>max) max=fabs(deltah);
           h[i]+=deltah;}}
       if (max<0.001) 
        {for (i=1;i<=lattice_size;i++)
          hold[i]=h[i];
          time+=factor;}
       else
        {h[i]=hold[i];
         factor/=3;}  
       if (max<0.0001) factor*=3;}
     fclose(fp1);
     fclose(fp2);
} 
