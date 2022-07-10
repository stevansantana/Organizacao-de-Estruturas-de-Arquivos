#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 8

typedef struct _Endereco Endereco;

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int compara(const void *e1, const void *e2)
{
	return strncmp(((Endereco*)e1)->cep,((Endereco*)e2)->cep,8);
}

int main()
{
    FILE *f, *saida;
    Endereco *e;
    long posicao, qtd, divide;
    char arqAux[20];
    f = fopen("cep.dat", "r");
    fseek(f, 0, SEEK_END);
    posicao = ftell(f);
    qtd = posicao/sizeof(Endereco);
    divide = qtd/N;

    // qtd = Quantidade total
    // divide = divide qtd em subdivisões

    // Ordenando cada subdivisão de cep.dat
    rewind(f);
    for(int i=0; i<N; i++)
    {
        if(i!=N-1)
        {
            e = (Endereco*)malloc(divide*sizeof(Endereco));
            if(fread(e, sizeof(Endereco), divide, f) == divide)
            {
                printf("%d. Lido = OK\n", i);
            }
            qsort(e, divide, sizeof(Endereco), compara);
            printf("%d. Ordenado = OK\n", i);
            sprintf(arqAux, "cep_%d.dat", i);
            saida = fopen(arqAux, "w");
            fwrite(e, sizeof(Endereco), divide, saida);
            fclose(saida);
            printf("Escrito = OK\n");
            free(e);
        }
        else
        {
            e = (Endereco*)malloc(divide*sizeof(Endereco));
            divide = divide + (qtd % N);
	        if(fread(e,sizeof(Endereco),divide,f) == divide){
            
		        printf("%d. Lido = OK\n", i);
	        }
	        qsort(e, divide, sizeof(Endereco), compara);
	        printf("%d. Ordenado = OK\n", i);
            sprintf(arqAux, "cep_%d.dat", i);
            saida = fopen(arqAux,"w"); 
	        fwrite(e,sizeof(Endereco),divide,saida);
	        fclose(saida);
	        printf("Escrito = OK\n");
	        free(e); 
        }
    }

    int inicio = 0, fim = N-1;
    // intercalando cada subdivisao até restar uma 
    while(inicio!=fim)
    {
        FILE *x, *y, *saida2;
        Endereco ex, ey;

        sprintf(arqAux, "cep_%d.dat", inicio);
	    x = fopen(arqAux,"r");

        sprintf(arqAux, "cep_%d.dat", inicio+1);
	    y = fopen(arqAux,"r");

        sprintf(arqAux, "cep_%d.dat", fim+1);
	    saida2 = fopen(arqAux,"w");

	    fread(&ex,sizeof(Endereco),1,x);
	    fread(&ey,sizeof(Endereco),1,y);

        while(!feof(x) && !feof(y))
        {
            if(compara(&ex, &ey)<0)
            {
                fwrite(&ex,sizeof(Endereco),1,saida2);
			    fread(&ex,sizeof(Endereco),1,x);
            }
            else
            {
                fwrite(&ey,sizeof(Endereco),1,saida2);
			    fread(&ey,sizeof(Endereco),1,y);
            }
        }
        while(!feof(x))
        {
            fwrite(&ex,sizeof(Endereco),1,saida2);
		    fread(&ex,sizeof(Endereco),1,x);
        }
        while(!feof(y))
        {
            fwrite(&ey,sizeof(Endereco),1,saida2);
		    fread(&ey,sizeof(Endereco),1,y);
        }

        fclose(x);
        fclose(y);
        fclose(saida2);

        inicio+=2;
        fim+=1;
    }

    printf("\nArquivo cep.dat ordenado pelo External Merge Sort.\n\n");
    return 0;
}
