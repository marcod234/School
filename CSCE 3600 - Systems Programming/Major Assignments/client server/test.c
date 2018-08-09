#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    char dump[50];
    float cpu;
    FILE *fp;
    pid_t pid = getpid();
    printf("pid = %d\n", pid);
    sprintf(dump, "top -b -p%d -n5 | grep %d | awk '{print $9}' > cpu", pid, pid);
    system(dump);
    fp = fopen("cpu", "r");
    fscanf(fp, "%f", &cpu);
    printf("CPU is %f\n", cpu);

 /*   fp = fopen("/proc/stat","r");
    fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&a[0],&a[1],&a[2],&a[3]);
    fclose(fp);
    sleep(1);

    fp = fopen("/proc/stat","r");
    fscanf(fp,"%*s %Lf %Lf %Lf %Lf",&b[0],&b[1],&b[2],&b[3]);
    fclose(fp);

    loadavg = ((b[0]+b[1]+b[2]) - (a[0]+a[1]+a[2]))
               / ((b[0]+b[1]+b[2]+b[3]) - (a[0]+a[1]+a[2]+a[3]));

    loadavg *= 100;
    printf("The current CPU utilization is : %Lf\n",loadavg);*/

    return(0);
}
