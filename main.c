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
	float transferValue;
} receipt;

struct Extract{
	float income;
	float withdrawal;
	char tm[50];
} extracts[100] = {};

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

	printf("Digite sua senha de acesso(ela deve conter exatos de 8 digitos): ");
	scanf("%d", &user.password);

	while(countDigits(user.password) != 8){
		printf("Sua senha tem que ter exatos 8 digitos!\n");
		printf("Digite sua senha de acesso: ");
		scanf("%d", &user.password);
	}
}

bool verifyPassword(int password){
	if(password != user.password){
		int trys = 3;
		while(password != user.password){
			if (trys <= 0){
				printf("Sua conta foi bloqueada! Entre em contato com o gerente do seu banco em uma unidade presencial!\n");
				exit(EXIT_SUCCESS);
			}

			printf("Senha incorreta! Você têm somente %d tentativas\n", trys);
			printf("Digite sua senha: ");
			scanf("%d", &password);
			trys--;
		}
	}

	return true;
}

bool login(){
	int password;
	printf("Digite sua senha: ");
	scanf("%d", &password);

	verifyPassword(password);

	printf("Bem vindo %s\n", user.name);
	return true;
}

bool changePassword(){
	int password;
	printf("Digite sua senha atual: ");
	scanf("%d", &password);

	verifyPassword(password);

	printf("Troca de senha aprovada!\n");
	printf("Digite sua nova senha: ");
	scanf(" %d", &user.password);

	while(countDigits(user.password) != 8){
		printf("Sua senha tem que ter exatos 8 digitos!\n");
		printf("Digite sua senha de acesso: ");
		scanf("%d", &user.password);
	}

	return true;
}

void menu(){
	int opt;
	time_t t = time(NULL);
	char yn;

	printf("\nMENU DE OPÇÕES.\n");
	printf("[1] Sacar dinheiro.\n");
	printf("[2] Depositar dinheiro.\n");
	printf("[3] Saldos e extratos.\n");
	printf("[4] Transferência.\n");
	printf("[5] Trocar senha.\n");
	printf("Escolha uma opção(somente no numero): ");
	scanf("%d", &opt);

	switch(opt){
		// Sacar dinheiro
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

		//Depositar dinheiro
		case 2: {
			float deposit;
			printf("Quanto deseja depositar? ");
			scanf("%f", &deposit);

			receipt.tm = localtime(&t);
			receipt.depositValue = deposit;
			user.income = user.income + deposit;
			printf("+------------------------------------------------------+\n");
			printf("| DATA: %s", asctime(receipt.tm));
			printf("| Valor depositado: %.2f\n", deposit);
			printf("| Valor na conta bancaria: %.2f\n", user.income);
			printf("+------------------------------------------------------+\n");

			printf("Deseja imprimir o comprovante? [s] Sim [n] Não.: ");
			scanf(" %c", &yn);

			if(yn == 's'){
				printf("Imprimindo...\n");
				printf("Impresso com sucesso! Dê uma olhada na sua impressora;\n");
			}
		} break;

		// Saldos e extratos
		case 3: {
			int opt2;
			printf("Saldos e extratos\n");
			printf("[1] Saldo simples.\n");
			printf("[2] Extrato.\n");
			printf("Excolha uma opção: ");
			scanf("%d", &opt2);

			struct tm *tm = localtime(&t);
			if(opt2 == 1){
				printf("+---------------------------------------------------------------------+\n");
				printf("| Seu saldo atual: %.2f\n", user.income);
				printf("| Seu nome no registro: %s\n", user.name);
				printf("| Data da atual situação do saldo: %s\n", asctime(tm));
				printf("+---------------------------------------------------------------------+\n");
			}
		} break;

		// Transferencias
		case 4: {
			char transferUser[50];
			float transferValue;
			printf("Digite o nome da pessoa o qual vai receber: ");
			scanf(" %[^\n]%*c", transferUser);

			printf("Quanto você vai transferir para %s: ", transferUser);
			scanf("%f", &transferValue);

			receipt.tm = localtime(&t);
			receipt.transferValue = transferValue;
			user.income = user.income - transferValue;
			printf("+------------------------------------------------------+\n");
			printf("| Tranferido de %s para %s\n", user.name, transferUser);
			printf("| DATA: %s", asctime(receipt.tm));
			printf("| Valor transferido: %.2f\n", receipt.transferValue);
			printf("| Valor na conta bancaria: %.2f\n", user.income);
			printf("+------------------------------------------------------+\n");

			printf("Deseja imprimir o comprovante? [s] Sim [n] Não.: ");
			scanf(" %c", &yn);

			if(yn == 's'){
				printf("Imprimindo...\n");
				printf("Impresso com sucesso! Dê uma olhada na sua impressora;\n");
			}
		} break;

		// Trocar senhas
		case 5: {
			if(changePassword()){
				printf("Senha trocada com sucesso!\n");
			}
		} break;

		default:
			printf("Operação inválida!\n");
		break;
	}

}

int main(){
	setlocale(LC_ALL, "Portuguese");

	printf("Bem vindo ao banco BIXA [Banco Internacional x Atual]. \n");
	printf("Para continuar você deve fazer seu registro!\n");

	signup();

	printf("Usuário criado com sucesso! Agora faça o login como %s!\n", user.name);

	login();

	// Logado

	while(true)
		menu();
}