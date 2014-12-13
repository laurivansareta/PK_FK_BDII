#include "buffend.h"

int main(int rg, char *nomeTabela[]){

	int erro;
	table *tDois = NULL;
	table *t = NULL;
	//column *c = NULL;
	tipoChave atributo;

	t = iniciaTabela("Profissao");
	if(t == ERRO_NOME_TABELA_INVALIDO){
		printf("Erro: na função iniciaTabela(). Nome da tabela já existente.\n");
		return 0;
	}
		atributo.tpChave = 1;
	t = adicionaCampo(t, "Nome", 'S', 20, &atributo);
		atributo.tpChave = 0;
	t = adicionaCampo(t, "descricao", 'S', 30, &atributo);
	t = adicionaCampo(t, "competencia", 'I', 20, &atributo);
	t = adicionaCampo(t, "experiencia", 'S', 25, &atributo);

	erro = finalizaTabela(t);

	//t = NULL;
	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela() - 1.\n", erro);
		return 0;
	}

	//inicia a tabela pessoa tendo como fk a profissao
	tDois = iniciaTabela("Pessoa");
		atributo.tpChave = 1;
	tDois = adicionaCampo(tDois, "Nome", 'S', 20, &atributo);
		atributo.tpChave = 0;
	tDois = adicionaCampo(tDois, "Idade", 'I', (sizeof(int)), &atributo);
	tDois = adicionaCampo(tDois, "Sexo", 'C', (sizeof(char)), &atributo);
	tDois = adicionaCampo(tDois, "Obs", 'S', 12, &atributo);
	tDois = adicionaCampo(tDois, "Media", 'D', (sizeof(double)), &atributo); //aqui ta gravando lixo deve ser por causa do tipo

	//	atributo.tpChave = 2;
	//	strcpy(atributo.nomeTabelaF, "Profissao");
	//	strcpy(atributo.nomeCampoF, "Nome");
	tDois = adicionaCampo(tDois, "profissao", 'S', 40, &atributo);

	erro = finalizaTabela(tDois);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela() - 2.\n", erro);
		return 0;
	}


/*
	c = insereValor(c, "Nome", "Um");
	c = insereValor(c, "Idade", "40");
	c = insereValor(c, "Sexo", "F");
	c = insereValor(c, "Obs", "Obs. Um");
	c = insereValor(c, "Media", "2.5");


	c = insereValor(c, "Nome", "Dois");
	c = insereValor(c, "Idade", "20");
	c = insereValor(c, "Sexo", "M");
	c = insereValor(c, "Obs", "Obs. Dois");
	c = insereValor(c, "Media", "1.67");

	c = insereValor(c, "Nome", "Três");
	c = insereValor(c, "Idade", "30");
	c = insereValor(c, "Sexo", "F");
	c = insereValor(c, "Obs", "Obs. Três");
	c = insereValor(c, "Media", "1.456");

	erro = finalizaInsert("Pessoa", c);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaInsert()\n", erro);
		return 0;
	}


	if(rg == 1){
		printf("Erro: nome de tabela nao inserido na linha de comando.\n");
		return 0;
	}

	struct fs_objects objeto = leObjeto(nomeTabela[1]);

	tp_table *esquema = leSchema(objeto);

	if(esquema == ERRO_ABRIR_ESQUEMA){
		printf("Erro ao criar o esquema.\n");
		return 0;
	}

	tp_buffer *bufferpoll = initbuffer();

	if(bufferpoll == ERRO_DE_ALOCACAO){
		printf("Erro ao alocar memória para o buffer.\n");
		return 0;
	}

	erro = colocaTuplaBuffer(bufferpoll, 0, esquema, objeto);

	if(erro != SUCCESS){
		printf("Erro %d: na função colocaTuplaBuffer().\n", erro);
		return 0;
	}

	erro = colocaTuplaBuffer(bufferpoll, 1, esquema, objeto);

	if(erro != SUCCESS){
		printf("Erro %d: na função colocaTuplaBuffer().\n", erro);
		return 0;
	}

	erro = colocaTuplaBuffer(bufferpoll, 2, esquema, objeto);

	if(erro != SUCCESS){
		printf("Erro %d: na função colocaTuplaBuffer().\n", erro);
		return 0;
	}

	column *tuplaE = excluirTuplaBuffer(bufferpoll, esquema, objeto, 0, 2); //pg, tupla
	column *pagina = getPage(bufferpoll, esquema, objeto, 0);

	if(tuplaE == ERRO_PARAMETRO){
		printf("Erro, na função excluirTuplaBuffer(), problemas no parametro.\n");
		return 0;
	}
	if(pagina == ERRO_PARAMETRO){
		printf("Erro, na função getPage(), problemas no parametro.\n");
		return 0;
	}

	// PARA IMPRIMIR TUPLA EXCLUIDA -----------------------------
	//-------------------------------------------------------------
	printf("\nTupla excluída do Buffer.\n");
	int j = 0;
	for(j=0; j < objeto.qtdCampos; j++){

		if(tuplaE[j].tipoCampo == 'S')
			printf("%s: %s ", tuplaE[j].nomeCampo,tuplaE[j].valorCampo);
		else if(tuplaE[j].tipoCampo == 'I'){
			int *n = (int *)&tuplaE[j].valorCampo[0];
			printf("%s: %d ",tuplaE[j].nomeCampo, *n);
		}
		else if(tuplaE[j].tipoCampo == 'C'){
			printf("%s: %c ",tuplaE[j].nomeCampo, tuplaE[j].valorCampo[0]);
		}
		else if(tuplaE[j].tipoCampo == 'D'){
			double *n = (double *)&tuplaE[j].valorCampo[0];
			printf("%s: %f ",tuplaE[j].nomeCampo, *n);
		}
		printf("\n");
	}

	//-------------------------------------------------------------

	// PARA IMPRIMIR PÁGINA ---------------------------------------
	//-------------------------------------------------------------
	printf("\nPágina armazenada na estrutura column *pagina.\n");
	for(j=0; j < objeto.qtdCampos*bufferpoll[0].nrec; j++){

		if(pagina[j].tipoCampo == 'S')
			printf("%s: %s ", pagina[j].nomeCampo,pagina[j].valorCampo);
		else if(pagina[j].tipoCampo == 'I'){
			int *n = (int *)&pagina[j].valorCampo[0];
			printf("%s: %d ",pagina[j].nomeCampo, *n);
		}
		else if(pagina[j].tipoCampo == 'C'){
			printf("%s: %c ",pagina[j].nomeCampo, pagina[j].valorCampo[0]);
		}
		else if(pagina[j].tipoCampo == 'D'){
			double *n = (double *)&pagina[j].valorCampo[0];
			printf("%s: %f ",pagina[j].nomeCampo, *n);
		}
		printf("\n");
	}
	printf("\n\n");

	//--------------------------------------------------------------


	erro = printbufferpoll(bufferpoll, esquema, objeto, 0);

	if(erro != SUCCESS){
		printf("Erro %d: na função printbufferpoll().\n", erro);
		return 0;
	}
	*/
	printf("\n");
	return 0;
}
