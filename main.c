#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE 100

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
	char type[20];
	char name[50];
	float inVal;
	float outVal;
	struct tm *tm;
};

struct Extract *extracts[MAX_SIZE];
int counter = 0;

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

void createExtract(
	char *type,
	char *name,
	float inVal,
	float outVal 
){
	time_t t = time(NULL);

	if(counter >= MAX_SIZE){
		printf("O array está cheio!");
		return;
	}

	/* id; type[20]; name; inVal; outVal; tm[50]; */

    struct Extract* newExtract = (struct Extract*)malloc(sizeof(struct Extract));


	strcpy(newExtract->type, type);
    strcpy(newExtract->name, name);
	newExtract->inVal = inVal;
	newExtract->outVal = outVal;
	newExtract->tm = localtime(&t);

	extracts[counter] = newExtract;
	counter++;
}

void withdraw(){
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
	createExtract("Saque", "", 0, value);
	printf("Você sacou %.2f com sucesso!", value);
}

void deposit(){
	float deposit;
	printf("Quanto deseja depositar? ");
	scanf("%f", &deposit);
time_t t = time(NULL);

	receipt.tm = localtime(&t);
	receipt.depositValue = deposit;
	user.income = user.income + deposit;
	createExtract("Deposito", "", deposit, 0);
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
}

void balance(){
	time_t t = time(NULL);
	struct tm *tm;
	printf("+---------------------------------------------------------------------+\n");
	printf("| Seu saldo atual: %.2f\n", user.income);
	printf("| Seu nome no registro: %s\n", user.name);
	printf("| Data da atual situação do saldo: %s\n", asctime(tm));
	printf("+---------------------------------------------------------------------+\n");
}

void listExtracts(){
	printf("SEUS EXTRATOS:\n");

	for(int i = 0; i < counter; i++){
		printf("+----------------------------------------------------------------+\n");
		printf("| Tipo: %s\n", extracts[i]->type);

		if(extracts[i]->name != "")
			printf("| Quem Recebeu: %s\n", extracts[i]->name);
		if(extracts[i]->inVal != 0)
			printf("| Valor de entrada: %.2f\n", extracts[i]->inVal);
		if(extracts[i]->outVal != 0)
			printf("| Valor de saida%.2f\n", extracts[i]->outVal);

		printf("| DATA: %s\n", asctime(extracts[i]->tm));

		printf("+----------------------------------------------------------------+\n");
	}
}

void transfers(){
	char transferUser[50];
	float transferValue;
	printf("Digite o nome da pessoa o qual vai receber: ");
	scanf(" %[^\n]%*c", transferUser);

	printf("Quanto você vai transferir para %s: ", transferUser);
	scanf("%f", &transferValue);
	
	time_t t = time(NULL);

	receipt.tm = localtime(&t);
	receipt.transferValue = transferValue;
	user.income = user.income - transferValue;
	printf("+------------------------------------------------------+\n");
	printf("| Tranferido de %s para %s\n", user.name, transferUser);
	printf("| DATA: %s", asctime(receipt.tm));
	printf("| Valor transferido: %.2f\n", receipt.transferValue);
	printf("| Valor na conta bancaria: %.2f\n", user.income);
	printf("+------------------------------------------------------+\n");

	createExtract("Tranferencia", transferUser, 0, transferValue);

	char yn;
	printf("Deseja imprimir o comprovante? [s] Sim [n] Não.: ");
	scanf(" %c", &yn);

	if(yn == 's'){
		printf("Imprimindo...\n");
		printf("Impresso com sucesso! Dê uma olhada na sua impressora;\n");
	}
}

void menu(){

	int opt;


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
			withdraw();
		} break;

		//Depositar dinheiro
		case 2: {
			deposit();
		} break;

		// Saldos e extratos
		case 3: {
			int opt2;
			printf("Saldos e extratos\n");
			printf("[1] Saldo simples.\n");
			printf("[2] Extrato.\n");
			printf("Excolha uma opção: ");
			scanf("%d", &opt2);
time_t t = time(NULL);

			struct tm *tm = localtime(&t);
			
			if(opt2 == 1){
				balance();
			}

			if(opt2 == 2){
				listExtracts();
			}
		} break;

		// Transferencias
		case 4: {
			transfers();
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

	return 0;
}