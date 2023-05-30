#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define max_noeud 200

typedef int bool;
#define true 1
#define false 0

typedef struct{
    int nbr_noeud;                         // Ce champs pour stocker le nombre de noeud
    char nom_noeud[max_noeud][30];         // Ce tableau des chaines de caracteres pour stocker les noms des noeud
    int matrice_poid[max_noeud][max_noeud];// Cette matrice pour stocker les arcs entre les neouds
}Graph;

void saisir_donnee(Graph* graph){
    int i,j,nbr;
    // la saisie du nombre des noeuds
    printf("entrez le nombre de noeud: ");
    scanf("%d",&graph->nbr_noeud);
    getchar();
    
    // la saisie des noms des noeuds
    for(i=0;i<graph->nbr_noeud;i++){
        printf("Entrez le nom du noeud %d: ",i+1);
        fgets(graph->nom_noeud[i], sizeof(graph->nom_noeud[i]) - 1, stdin);
        // Supprimer le caractère de nouvelle ligne
        graph->nom_noeud[i][strcspn(graph->nom_noeud[i], "\n")] = '\0';
    }

    // Remplissage de la matrice de poid 
    printf("Donnez les arcs entre chaque deux noeud, et s'il n'est pas d'arc tapez 0\n");
    for(i=0;i<(graph->nbr_noeud)-1;i++)
    {
        for(j=i+1;j<graph->nbr_noeud;j++)
        {
            printf(" %s <--> %s : ",graph->nom_noeud[i],graph->nom_noeud[j]);
            scanf("%d",&graph->matrice_poid[i][j]);

            // l'ajout de la même valeur dans la position symétriqu
            graph->matrice_poid[j][i] = graph->matrice_poid[i][j];
        }
    }

}

typedef struct List{
    char noeud[30];
    struct List *suivant;
}List;

void ajout_tete(List **tete, char element[30]) // Fonction qui ajout un element en tete d'une liste
{
    List *nouveau_element=malloc(sizeof(List));
    strcpy(nouveau_element->noeud,element);
    nouveau_element->suivant=*tete;
    *tete=nouveau_element;
}

void calcul_short_way(Graph* graph)
{
    char depart[30],arrive[30];
    printf("Entrez le point de depart: ");
    scanf("%s", depart);
    printf("Entrez le point d'arrivee: ");
    scanf("%s", arrive);

    int index_depart = -1;
    int index_arrive = -1;

    // Preciser les index correspondants aux noeuds de depart et d'arrivee
    for(int i=0;i<graph->nbr_noeud;i++)
    {
        if(strcmp(graph->nom_noeud[i],depart) == 0)
        {
            index_depart = i;
        }
        if(strcmp(graph->nom_noeud[i],arrive) == 0)
        {
            index_arrive = i;
        }
    }

    // Tester la condition de si le noeud de depart ou d'arrivee ou les deux sont introuvables
    if(index_depart == -1 || index_arrive == -1)
    {
        printf("Le noeud de depart ou d'arrivee est introvable! ");
        return;
    }

    // initialisation de tous les noeuds en infini, sauf le noeud de dapart
    int distance[max_noeud];
    bool visited[max_noeud];
    int precedent[max_noeud];
    for(int i=0; i<graph->nbr_noeud; i++)
    {
        if(i == index_depart)
            distance[i] = 0;
        else
            distance[i] = INT_MAX;
    }

    // Marquer tous les noeuds comme non visité

    for(int i=0; i<graph->nbr_noeud; i++)
    {
        precedent[i] = -1;
        visited[i] = false;
    }

    for(int i=0; i<(graph->nbr_noeud)-1; i++)
    {
        int v = -1;
        int min_distance = INT_MAX;

        for(int j=0; j<graph->nbr_noeud; j++)
        {
            if(!visited[j] && distance[j] < min_distance)
            {
                v = j;
                min_distance = distance[j];
            }
        }

        if(v == -1)
        {
            break; // tous les noeuds sont visités
        }
        // v represente l'indice du noeud actuel et w represente l'indice du noeud adjacent
        visited[v] = true;

        for(int w=0 ;w<graph->nbr_noeud; w++)
        {
            int arc = graph->matrice_poid[v][w];
            if(arc != 0 && !visited[w] && distance[v] != INT_MAX && (distance[v] + arc) < distance[w])
            {
                distance[w] = distance[v] + arc;
                precedent[w] = v;
            }
        }
    }
    // on construire le chemin le plus court 
    if(distance[index_arrive] == INT_MAX) // tester si aucun chemin n'a été trouvé
    {
        printf("Aucun chemin entre %s et %s .",depart,arrive);
    }
    else
    {
        List* chemin = NULL;
        int index = index_arrive;

        while(index != -1)
        {
            ajout_tete(&chemin, graph->nom_noeud[index]);
            index = precedent[index];
        }

        // Affichage du chemin 
        printf("Le chemin le plus court de %s a %s est : ", depart, arrive);
        List* actuel = chemin;
        while( actuel != NULL)
        {
            printf("%s ", actuel->noeud);
            actuel = actuel->suivant;
            if( actuel != NULL)
            printf("-> ");
        }
        printf("\n");

        //Liberation de la memoire utilisé pour la liste chemin
        while( chemin != NULL)
        {
            List* tmp =chemin;
            chemin = chemin->suivant;
            free(tmp);
        }
    }
}
int main()
{
    debut:
    Graph my_graph;
    saisir_donnee(&my_graph);
    calcul_short_way(&my_graph);
    
    question:
    char val;
    printf("\nEst-ce-que vous pouvez demarrer une autre recherche? Si oui tapez Y ou y sinon tapez N ou n : ");
    scanf(" %c", &val);
    switch(val)
    {
        case 'y':
        case 'Y':
            goto debut;
            break;
        case 'n':
        case 'N':
            printf("Fin d'executuion!");
            break;
        default: 
            goto question;
            break;
    }
    return 0;
}