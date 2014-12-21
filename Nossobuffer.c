#include "buffend.h"

int main(){

	int erro;
	table *t = NULL;
	table *tDois = NULL;
	column *c = NULL;
	column *cDois = NULL;	
	tipoChave atributo;
	
	//nome das tabelas
	char nomeTabela[20] = "pessoa";
	char nomeTabelaDois[20] = "componente";
	
//---------------Tabela pessoa ------------------	
	t = iniciaTabela(nomeTabela);

	if(t == ERRO_NOME_TABELA_INVALIDO){
		printf("Erro: na função iniciaTabela(). Nome da tabela já existente.\n");
		return 0;
	}
	
	atributo.tpChave = 1;	
	t = adicionaCampo(t, "Nome", 'S', 20, &atributo);
	t = adicionaCampo(t, "Idade", 'I', (sizeof(int)), &atributo);
	t = adicionaCampo(t, "Sexo", 'C', (sizeof(char)), &atributo);
	t = adicionaCampo(t, "Media", 'D', (sizeof(double)), &atributo);
	atributo.tpChave = 2;
	t = adicionaCampo(t, "matricula", 'I', (sizeof(int)), &atributo);
	
	erro = finalizaTabela(t);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela().\n", erro);
		return 0;
	}
	
//--------------Tabela componente -------------------
	tDois = iniciaTabela(nomeTabelaDois);

	if(tDois == ERRO_NOME_TABELA_INVALIDO){
		printf("Erro: na função iniciaTabela(). Nome da tabela já existente.\n");
		return 0;
	}
	
	atributo.tpChave = 2;
	strcpy(atributo.nomeTabelaF, nomeTabela );
	strcpy(atributo.nomeCampoF, "matricula" );
	tDois = adicionaCampo(tDois, "matricula", 'I', (sizeof(int)), &atributo);
	atributo.tpChave = 1;
	tDois = adicionaCampo(tDois, "nomeComp", 'S', 20, &atributo);
	tDois = adicionaCampo(tDois, "professor", 'S', 20, &atributo);
	tDois = adicionaCampo(tDois, "qtAlunos", 'I', (sizeof(int)), &atributo);
	tDois = adicionaCampo(tDois, "Obs", 'S', 20, &atributo);
	tDois = adicionaCampo(tDois, "MediaGeral", 'D', (sizeof(double)), &atributo);

	erro = finalizaTabela(tDois);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaTabela().\n", erro);
		return 0;
	}
	
//----------------------
	
	c = insereValor(c, "Nome", "Douglas");
	c = insereValor(c, "Idade", "28");
	c = insereValor(c, "Sexo", "F");
	c = insereValor(c, "Media", "5.0");
	c = insereValor(c, "matricula", "10");

	c = insereValor(c, "Nome", "lucas");
	c = insereValor(c, "Idade", "24");
	c = insereValor(c, "Sexo", "I");
	c = insereValor(c, "Media", "4.5");
	c = insereValor(c, "matricula", "11");
	
	c = insereValor(c, "Nome", "Laurivan");
	c = insereValor(c, "Idade", "21");
	c = insereValor(c, "Sexo", "M");
	c = insereValor(c, "Media", "9.9");
	c = insereValor(c, "matricula", "12");
	//finaliza tabela pessoa
	erro = finalizaInsert(nomeTabela, c);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaInsert()\n", erro);
		return 0;
	}
//----------------- testa a funcao getTabela()------------------	
	table *teste;
	teste = getTabela(nomeTabela);
	printf("\n\n nome tabela:%s\n\n",teste->nome); 


	cDois = insereValor(cDois, "matricula", "10");
	cDois = insereValor(cDois, "nomeComp", "bancoII");
	cDois = insereValor(cDois, "professor", "Denio");
	cDois = insereValor(cDois, "qtAlunos", "20");
	cDois = insereValor(cDois, "Obs", "alunos maravilhosos");
	cDois = insereValor(cDois, "MediaGeral", "8.6");
	
	cDois = insereValor(cDois, "matricula", "11");
	cDois = insereValor(cDois, "nomeComp", "Eng Soft");
	cDois = insereValor(cDois, "professor", "Raquel");
	cDois = insereValor(cDois, "qtAlunos", "25");
	cDois = insereValor(cDois, "Obs", "estudando");
	cDois = insereValor(cDois, "MediaGeral", "7.5");
	
	//finaliza tabela componente
	erro = finalizaInsert(nomeTabelaDois, cDois);

	if(erro != SUCCESS){
		printf("Erro %d: na função finalizaInsert()\n", erro);
		return 0;
	}
	
	//para testar o exclui tabela
	//excluiTabela(nomeTabela);
	
printf("\n\n\n -------------------------------- TABELA PESSOA----------------------\n");
//tabela pessoa inicialização do buffer e impressão dos dados
	struct fs_objects objeto = leObjeto(nomeTabela);
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
//------------------- buffer 

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
	int j;
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
//---------------------fim da impresso da tabela pessoa --------------------------
printf("\n\n\n -------------------------------- TABELA COMPONENTE----------------------\n");
//tabela componente inicialização do buffer e impressão dos dados
	struct fs_objects objetoDois = leObjeto(nomeTabelaDois);
	tp_table *esquemaDois = leSchema(objetoDois);

	if(esquemaDois == ERRO_ABRIR_ESQUEMA){
		printf("Erro ao criar o esquema.\n");
		return 0;
	}

	tp_buffer *bufferpollDois = initbuffer();

	if(bufferpollDois == ERRO_DE_ALOCACAO){
		printf("Erro ao alocar memória para o buffer.\n");
		return 0;
	}

	erro = colocaTuplaBuffer(bufferpollDois, 0, esquemaDois, objetoDois);

	if(erro != SUCCESS){
		printf("Erro %d: na função colocaTuplaBuffer().\n", erro);
		return 0;
	}

	erro = colocaTuplaBuffer(bufferpollDois, 1, esquemaDois, objetoDois);

	if(erro != SUCCESS){
		printf("Erro %d: na função colocaTuplaBuffer().\n", erro);
		return 0;
	}

	column *paginaDois = getPage(bufferpollDois, esquemaDois, objetoDois, 0);

	if(paginaDois == ERRO_PARAMETRO){
		printf("Erro, na função getPage(), problemas no parametro.\n");
		return 0;
	}
	// PARA IMPRIMIR PÁGINA ---------------------------------------
	//-------------------------------------------------------------
	printf("\nPágina armazenada na estrutura column *pagina.\n");
	for(j=0; j < objetoDois.qtdCampos*bufferpollDois[0].nrec; j++){

		if(paginaDois[j].tipoCampo == 'S')
			printf("%s: %s ", paginaDois[j].nomeCampo,paginaDois[j].valorCampo);
		else if(paginaDois[j].tipoCampo == 'I'){
			int *n = (int *)&paginaDois[j].valorCampo[0];
			printf("%s: %d ",paginaDois[j].nomeCampo, *n);
		}
		else if(paginaDois[j].tipoCampo == 'C'){
			printf("%s: %c ",paginaDois[j].nomeCampo, paginaDois[j].valorCampo[0]);
		}
		else if(paginaDois[j].tipoCampo == 'D'){
			double *n = (double *)&paginaDois[j].valorCampo[0];
			printf("%s: %f ",paginaDois[j].nomeCampo, *n);
		}
		printf("\n");
	}
	printf("\n\n");

	erro = printbufferpoll(bufferpollDois, esquemaDois, objetoDois, 0);

	if(erro != SUCCESS){
		printf("Erro %d: na função printbufferpoll().\n", erro);
		return 0;
	}
	
	printf("\n");
	return 0;
}
