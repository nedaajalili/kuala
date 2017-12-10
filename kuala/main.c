#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct k
{
    char kId[2];
    int nt;
    int *tree;
    int hometree;
    char family[2];
} koala;
typedef struct f
{
    char fId[2];
    int nmember;
    koala *koalas;
    char enemy[2];
} family;
void showargs(int,int,int,int);
FILE *openF(char *,char *);
family *readFamily(int);
koala *readkoala(int);
int checksol(koala *,family *,int,int,int,int,int);

int main(int argc,char *argv[])
{
    family *families;
    koala *koalas;
    int nt,nf,nk,n,sol;
    if(argc!=5)
    {
        printf("error in command line arguments!\n");
        exit(0);
    }
    nt=atoi(argv[1]);
    nf=atoi(argv[2]);
    nk=atoi(argv[3]);
    n=atoi(argv[4]);
    showargs(nt,nf,nk,n);

    koalas=readkoala(nk);
    families=readFamily(nf);
    sol=checksol(koalas,families,nt,nf,nk,n,0);
    if(sol)
    {
        printf("solution is valid.\n");
    }
    else
    {
        printf("Not valid!\n");
    }

    return 1;
}
void showargs(int nt,int nf,int nk,int n)
{
    printf("number of trees is:%d \n",nt);
    printf("number of family is:%d \n",nf);
    printf("total number of koalas is:%d \n",nk);
    printf("number of koalas on each tree is:%d \n",n);
    return;
}
FILE *openF(char *fn,char *mode)
{
    FILE *fp;
    fp=fopen(fn,mode);
    if(fp==NULL)
    {
        printf("error in opening file %s",fn);
        exit(0);
    }
    return fp;
}
family *readFamily(int nf)
{
    FILE *ffp, *efp;
    char e1[2],e2[2];
    int i,j;
    family *families;
    families=(family *)malloc(nf*sizeof(family));
    if(families==NULL)
    {
        printf("error in allocation!\n");
        exit(0);
    }
    ffp=openF("families","r");
    i=0;
    while(i<nf && fscanf(ffp,"%s %d",families[i].fId, &families[i].nmember)!=EOF)
    {
        families[i].koalas = (koala *) malloc(families[i].nmember * sizeof(koala));
        if(families[i].koalas==NULL)
        {
            printf("error in allocation!\n");
            exit(0);
        }
        for(j=0; j<families[i].nmember; j++)
        {
            fscanf(ffp,"%s",families[i].koalas[j].kId);
            strcpy(families[i].koalas[j].family,families[i].fId);
        }
        efp=openF("enemies","r");
        while(fscanf(efp,"%s %s",e1,e2)!=EOF)
        {
            if(!strcmp(families[i].fId,e1))
            {
                strcpy(families[i].enemy,e2);
            }
            else if(!strcmp(families[i].fId,e2))
            {
                strcpy(families[i].enemy,e1);
            }
        }
        fclose(efp);
        i++;
    }
    if(i!=nf)
    {
        printf("error in file format!");
        exit(0);
    }
    fclose(ffp);
    return families;
}
koala *readkoala(int nk)
{
    FILE *kfp;
    koala *koalas;
    int i,j;
    koalas=(koala *)malloc(nk*sizeof(koala));
    if(koalas==NULL)
    {
        printf("error in allocation!\n");
        exit(0);
    }
    kfp=openF("habitat","r");
    i=0;
    while(i<nk && fscanf(kfp,"%s %d",koalas[i].kId, &koalas[i].nt)!=EOF)
    {
        for(j=0; j<koalas[i].nt; j++)
        {
            fscanf(kfp,"%d", &koalas[i].tree[j]);
        }
        koalas[i].hometree=-1;
        i++;
    }
    if(i!=nk)
    {
        printf("error in file format!");
        exit(0);
    }
    fclose(kfp);
    return koalas;
}
int checksol(koala *koalas,family *families,int nt,int nf,int nk,int n,int level){
    FILE *sfp;
    char koal[2];
    int tree[nt][n];
    int i,j,k,t,hometree;
    for(i=0;i<nt;i++){
        for(j=0;j<n;j++){
           tree[i][j]=0;
        }
    }
    sfp=openF("solution","r");
    while(fscanf(sfp,"%s %d",koal, &hometree)!=EOF){
        for(i=0;i<nt;i++){
            t=0;
            for(j=0;j<nk;j++){
                if(!strcmp(koalas[j].kId,koal)){
                    for(k=0;k<koalas[j].nt;k++){
                        if(hometree==(koalas[j].tree[k])){
                            koalas[j].hometree=hometree;
                            tree[hometree][t]=1;
                            t++;
                        }
                    }
                }
            }
        }
    }
    for(i=0;i<nt;i++){
        for(j=0;j<n;j++){
           if(tree[i][j]==0){
            return 0;
           }
        }
    }
    fclose(sfp);
    return 0;
}


