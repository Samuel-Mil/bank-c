#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

struct Users {
	char name[50];
	float income;
	int password;
} user;

// Comprovantes
struct Receipt{
	struct tm *tm;
	float depositValue;
} receipt;

/*
 r = {
	{"dt", 300},
	{"dt", 300},
	{"dt", 300}
 }
*/

int countDigits(int num) {
    int count = 0;
    while (num != 0) {
        num /= 10;
        count++;
    }
    return count;
}

void signup(){
	printf("Digite o seu nome completo: ");
	scanf("%[^\n]%*c", user.name);

	printf("Digite sua renda mensal: ");
	scanf("%f", &user.income);

	printf("Digite sua senha de acesso(ela deve conter mais de 8 digitos): ");
	scanf("%d", &user.password);

	while(countDigits(user.password) < 8){
		printf("Sua senha tem que ser maior que 8 digitos!\n");
		printf("Digite sua senha de acesso: ");
		scanf("%d", &user.password);
	}
}

bool login(){
	int password;
	printf("Digite sua senha: ");
	scanf("%d", &password);

	if(password != user.password){
		int trys = 3;
		while(password != user.password){
			if (trys <= 0){
				printf("Sua conta foi bloqueada! Entre em contato com o gerente do seu banco em uma unidade presencial!\n");
				return false;
			}

			printf("Senha incorreta! Você têm somente %d tentativas\n", trys);
			printf("Digite sua senha: ");
			scanf("%d", &password);
			trys--;
		}
	}

	printf("Bem vindo %s\n", user.name);
	return true;
}

void menu(){
	int opt;
	printf("\nMENU DE OPÇÕES.\n");
	printf("[1] Sacar dinheiro.\n");
	printf("[2] Depositar dinheiro.\n");
	printf("[3] Saldos e extratos.\n");
	printf("[4] Transferência.\n");
	printf("[5] Trocar senha.");
	printf("Escolha uma opção(somente no numero): ");
	scanf("%d", &opt);

	switch(opt){
		case 1: {
			float value;
			printf("Quanto deseja sacar? ");
			scanf("%f", &value);
			while(value > user.income){
				printf("Você não pode sacar mais do que tem!\n");
				printf("Você possui %.2f\n", user.income);
				printf("Quanto deseja sacar? ");
				scanf("%f", &value);
			}

			user.income = user.income - value;
			printf("Você sacou %.2f com sucesso!", value);
		} break;

		case 2: {
			float deposit;
			printf("Quanto deseja depositar? ");
			scanf("%f", &deposit);

			time_t t = time(NULL);
			receipt.tm = localtime(&t);
			receipt.depositValue = deposit;
			user.income = user.income + deposit;
			printf("+------------------------------------------------------+\n");
			printf("| DATA: %s", asctime(receipt.tm));
			printf("| Valor depositado: %.2f\n", deposit);
			printf("| Valor na conta bancaria: %.2f\n", user.income);
			printf("+------------------------------------------------------+\n");

			char yn;
			printf("Deseja imprimir o comprovante? [s] Sim [n] Não.: ");
			scanf(" %c", &yn);

			if(yn == 's'){
				printf("Imprimindo...\n");
				printf("Impresso com sucesso! Dê uma olhada na sua impressora;\n");
			}
		} break;
	}

}

int main(){
	setlocale(LC_ALL, "Portuguese");

	printf("Bem vindo ao banco BIXA [Banco Internacional x Atual]. \n");
	printf("Para continuar você deve fazer seu registro!\n");

	signup();

	printf("Usuário criado com sucesso! Agora faça o login como %s!\n", user.name);

	if(!login()){
		return 0;
	}

	// Logado

	while(true)
		menu();
}