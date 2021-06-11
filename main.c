/**
 * @mainpage
 * <center>
 * Strona zawiera dokumentacje kodu źródłowego programu <h1>DPC</h1> (Dijkstra Path Calculator)<br/>
 * Kod źródłowy: https://github.com/skrzypski/DPC
 * </center>
**/
/**
* @file main.c
* @brief Plik z głównym kodem programu
**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma warning(disable:4996)


/**
* @brief
* Jednokierunkowa lista liniowa przechowująca dane potrzebne do obliczenia najkrótszej ścieżki
**/
typedef struct slistEl 
{
	struct slistEl* next; ///< Następny element typu slistEl
	int dest; ///< Cel ścieżki
	int cost; ///< Waga ścieżki
} Graph;

char mapFile[128]; ///< Zmienna przechowująca nazwę pliku z grafem

/**
* @brief
* Główna funkcja programu
**/
int main(int argc, char* argv[])
{
	int i = 0;
	int j = 0;
	int edgeCount = 0;
	int pointCount = 0;
	int start = 0;
	int stop = 0;
	int u = 0;
	int w = 0;
	int x = 0;
	int y = 0;
	int Stackptr = 0;
	int* costs = NULL;
	int* prev = NULL;
	int* Stack = NULL;

	bool* QS = NULL;

	Graph** graph = NULL;
	Graph* pw = NULL;
	Graph* rw = NULL;

	if (argc == 1)
	{
		printf("Pliki z mapami w obecnym katalogu:\n");
		system("dir *.dmap /b");
		printf("Podaj nazwe pliku bez rozszerzenia: ");
		fgets(mapFile, 128, stdin);
		mapFile[strlen(mapFile) - 1] = NULL;
		strcat(mapFile, ".dmap");
	}
	else if (argc == 2)
	{
		strcpy(mapFile, argv[1]);
	}
	else
	{
		printf("Niewlasciwa liczba argumentow!\n");
		system("pause");
		return 0;
	}
	//---------------------------------------------------	   	
	FILE* file = fopen(mapFile, "r");
	if (file == NULL)
	{
		printf("Nie ma takiego pliku!!!!\n");
		system("pause");
		return 0;
	}
	char fileBuffer[1024];
	file = fopen(mapFile, "r");
	while (fgets(fileBuffer, 1024, file)) pointCount++;
	fseek(file, 0, SEEK_SET);
	//---------------------------------------------------
	costs = (int*)calloc(pointCount, sizeof(int));
	prev = (int*)calloc(pointCount, sizeof(int));
	Stack = (int*)calloc(pointCount, sizeof(int));
	QS = (bool*)calloc(pointCount, sizeof(bool));
	graph = (struct slistEl**)calloc(pointCount, sizeof(struct slistEl));
	//---------------------------------------------------   	
	for (i = 0; i < pointCount; i++)
	{
		costs[i] = 2147483647;
		prev[i] = -1;
		QS[i] = false;
		graph[i] = NULL;
	}
	//---------------------------------------------------
	while (fgets(fileBuffer, 1024, file))
	{
		fileBuffer[strlen(fileBuffer) - 1] = NULL;
		int semicolonCount = 0;
		for (int i = 0; i < strlen(fileBuffer); i++)
		{
			if (*(fileBuffer + i) == ';') semicolonCount++;
		}
		char* tokBuff = strtok(fileBuffer, "{,;}");
		int ID = atoi(tokBuff);
		ID -= 1;
		printf("%d -> ", ID);
		tokBuff = strtok(NULL, "{,;}");
		while (tokBuff != NULL)
		{
			int Dest = atoi(tokBuff);
			Dest -= 1;
			printf("%d, ", Dest);
			tokBuff = strtok(NULL, "{,;}");
			int Cost = atoi(tokBuff);
			printf("%d; ", Cost);
			tokBuff = strtok(NULL, "{,;}");
			edgeCount++;
			pw = (Graph*)calloc(1, sizeof(Graph));
			pw->dest = Dest;
			pw->cost = Cost;
			pw->next = graph[ID];
			graph[ID] = pw;
		}
		printf("\n");
	}
	fclose(file);
	//---------------------------------------------------
	printf("Podaj punkt poczatkowy: ");
	scanf("%d", &start);

	printf("Podaj punkt koncowy: ");
	scanf("%d", &stop);

	costs[start] = 0;

	for (i = 0; i < pointCount; i++)
	{
		for (j = 0; QS[j]; j++);

		for (u = j++; j < pointCount; j++)
		{
			if (!QS[j] && (costs[j] < costs[u])) u = j;
		}

		QS[u] = true;

		for (pw = graph[u]; pw; pw = pw->next)
		{
			if (!QS[pw->dest] && (costs[pw->dest] > costs[u] + pw->cost))
			{
				costs[pw->dest] = costs[u] + pw->cost;
				prev[pw->dest] = u;
			}
		}
	}

	for (i = 0; i < pointCount; i++)
	{
		if (i == stop)
		{
			printf("START: %d\n", start);

			printf("Sciezka: ");

			for (j = i; j > -1; j = prev[j]) Stack[Stackptr++] = j;

			while (Stackptr) printf("%d ", Stack[--Stackptr]);

			printf("\nKOSZT: %d\n", costs[i]);
		}
	}
	//---------------------------------------------------
	free(costs);
	free(prev);
	free(Stack);
	free(QS);
	for (i = 0; i < pointCount; i++)
	{
		pw = graph[i];
		while (pw)
		{
			rw = pw;
			pw = pw->next;
			free(rw);
		}
	}
	free(graph);
	system("pause");
}