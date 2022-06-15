#include <stdio.h>
#include <string.h>

typedef struct _Endereco Endereco;

// registroCEP = struct.Struct("72s72s72s72s2s8s2s")

struct _Endereco
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2]; // Ao Espaço no final da linha + quebra de linha
};

int main(int argc, char**argv)
{
    FILE *f;
	Endereco e;
    int qtd=0;
    long inicio=0, fim, meio, tamArqBits, tamRegistro;

    if(argc != 2)
	{
		fprintf(stderr, "USO: %s [CEP]", argv[0]);
		return 1;
    }

    f = fopen("cep_ordenado.dat", "r");
    if(f==NULL)
    {
        printf("Erro na abertura do arquivo!\n");
        system("pause");
        return 1;
    }

    fseek(f, 0, SEEK_END);
    tamArqBits = ftell(f);
    tamRegistro = tamArqBits/sizeof(Endereco);
    fim = tamRegistro-1;

    while(fim>=inicio)
    {
        meio=(inicio+fim)/2;
        fseek(f, (meio*sizeof(Endereco)), SEEK_SET);
        fread(&e, sizeof(Endereco), 1, f);
        qtd++;

        if(strncmp(argv[1],e.cep,8) == 0)
        {
            printf("Cep encontrado:\n");
            printf("%.72s\n%.72s\n%.72s\n%.72s\n%.2s\n%.8s\n",e.logradouro,e.bairro,e.cidade,e.uf,e.sigla,e.cep);
            break;
        }
        else
        {
            if(strncmp(argv[1],e.cep,8) > 0)
            {
                inicio=meio+1;
            }
            else
            {
                fim=meio-1;
            }
        }
    }
    printf("Quantidade de leituras: %d\n", qtd);
    fclose(f);

    return 0;
}