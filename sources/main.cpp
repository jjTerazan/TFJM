#include "TFJM.h"
#include "Node.h"
#include "Pile.h"
#include "NodePile.h"
#include "Table.h"


long factorial(long n)
{
    long ret = n--;
    while (n > 1) ret *= n--;
    return ret;
}


u8* initialize(const u32 nbcandidats)
{
    //u8* pmx = (u8*)

    return nullptr;
}

/*
Node* BuildLine1(const u8* pkl, const u8* ptarg)
{
    NodePile pile{2048}; // L'objet Pile créé est local mais la mémoire allouée par Pile se fait via un malloc


    // Pour accélérer les recherches à venir on construit un tableau temporaire où on stocke la position de chaque joueur sur la ligne 0
    u8 line0[PLAYERNB];
    line0[0]                        = 0;                // 0 est en début de ligne , of course
    line0[PLAYERNB - 1]             = ptarg[0];  // l'objectif du joueur 0 est inséré en fin de ligne, of course
    u8 posinlin0[PLAYERNB];
    posinlin0[0]                    = 0;                // 0 est en début de ligne , of course
    posinlin0[ptarg[0]]             = PLAYERNB-1;       // l'objectif du joueur 0 est inséré en fin de ligne, of course

    for (u8 i = 0; i < PLAYERNB - 2; i++)
    {
        line0[i + 1]        = pkl[i];          // On inscrit la liste de kills dans une liste comprenant 0 en tête et l'objectif en fin.   
        posinlin0[pkl[i]]    = i + 1;     // La position du joueur pkl[i] sur la ligne 0 est tout simplement i+1 ( + 1 car on intègre dans cette liste le joeur 0 et l'objectif du joueur 0 )  
    }

    // Pour accélérer les recherches on construit un tableau qui décrit, pour chaque joueur la liste des victimes possibles en prenant en compte
    // l'historique des lignes précédentes:
    // pour la ligne 0 lchunk[n] correspond à l'ensemble des joueurs situés avant n + le joueur situé juste après lui
    u16 lchunk[PLAYERNB];
    for (u8 i = 0; i < PLAYERNB; i++)
    {
        // on set à 1 le bit du joueur situé juste après le joueur i sur la ligne 0
        if (posinlin0[i] < PLAYERNB - 1)
            lchunk[i] = 1 << line0[posinlin0[i] + 1];
        // sauf pour le joueur i situé sur la dernière cellule de la ligne 0 ( et qui est donc l'objectif )
        else
        {
            assert(i == ptarg[0]); // on vérifie que i est bien l'objectif de la ligne 0
            lchunk[i] = 0;
        }
        // on set à 1 le bit n de chaque joueur (n = line0[j], joueur situé en position j sur la ligne 0 )
        for (u8 j = 0; j < posinlin0[i]; j++)
            lchunk[i] |= 1 << line0[j];


        assert(!((lchunk[i] >> i) & 1));
    }
    // Les joueurs envisageables comme second noeud sont tous ceux situés dans l'intervalle [0, pos[1]+1] sur la ligne 0.
    // On utilisera:
    //                  line0[] où chacune des cellules contient le nom d'une victime, de la premiere à la derniere
    //                  posinlin0[] où chaque cellule contient la position du joueur dans line0. Ainsi posinlin0[2]=  3 signifie que line0[3]=2 ! 
    //                  lifeline où la valeur de chaque bit indique si le joueur de nom BIT_n est mort ou vif sur la ligne 1

    // On construit la ligne 1 dont le premier noeud est le joueur 1
    //
    u8 currentobjectif = ptarg[1]; // ptarg[1] est l'objectif du joueur 1

    Node* pn0 = new Node(1);
    Node* pn;
    u16 hint, insert;
    u8 p;
    // On push ce noeud sur la pile et Go
    pile.push(pn0);
    while (!pile.isEmpty())
    {
        pn = pile.pop();
        // rechecherche pn0->id dans line0_chunk
        hint = lchunk[pn->m_Id] & pn->m_isDead;
        assert(!((hint >> pn->m_Id)&1)); //  le bit m_Id est à 1 dans m_isDead, cependant il est (normalement) à 0 dans lchunk donc à 0 dans hint !
        // Si il y a une intersection entre lchunk et isDead, alors...
        if (hint)
        {
            // on insère tout le monde sauf les ancetres, sois même et l'objectif courant
            insert = ~(pn->m_isDead | (1 << currentobjectif));
        }
        // Sinon, si on ne détecte aucune intersection
        else
        {
            // on insère lchunk
            insert = lchunk[pn->m_Id]& ~(1<< currentobjectif);
        }

        // on insère comme joueur possible tous les joueurs dont les bits sont à 1 dans insert
        // ... et on "empile" les nodes créés sur la pile récursive explicite
        for (p = 0; p < PLAYERNB; p++)
        {
            if ((insert >> p) & 1)
            {
                pile.push(new Node(p, pn));
            }
        }
    }

    return pn0;
 }
*/



int main()
{
    // Initialize PErformance Timer
    u64			timestart, timeend, frequency;
    f64			time, totaltime,lefttime, invfrequency;
    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    invfrequency = (f64)1 / frequency;


    // Start Performance Timer 
    QueryPerformanceCounter((LARGE_INTEGER*)&timestart);
    
    // The Loop
    //  PLAYERNB;                   // Le nombre de joueurs. le premier joueur se nomme 0, le second se nomme 1,...
    //                                 On utilise ici un define pour simplifier et pouvoir déclarer en static un maximum de table. 

    // 1)   Creation de la liste de toutes les premières lignes de kills possibles
    //      Chaque ligne possible correspond à l'enchainement "qui tue qui" en partant du joueur 0 pour atteindre en fin de ligne l'objectif du joueur 0
    //      On a donc,  ligne[0]            = 0 ( i.e 'début de ligne est joueur 0' )
    //                  ligne[player_nb-1]  = target[0] ( i.e 'fin de ligne est objectif du joueur 0' )
    //      Ces premières lignes possibles commenceront toutes par 0 et se termineront toutes pas target[0]
    //      Entre ces deux extrémités on comptera, "player_nb - 2" cellules.
    //      L'ensemble des lignes possibles de "player_nb - 2" cellules correspond à l'ensemble des permutations réalisables avec
    //      le groupe de base ]0,1,2,..,player_nb-1] -  target[0] comptant donc "player_nb - 2" éléments.
    //      On construit donc ce groupe et on liste toutes les permutations.
    
    // ***************************************************************************************************************************************
    // *
    // *    INITIALISATIONS
    // *
    u8    target[PLAYERNB]      = { TARGETS_LIST     }; // La table des objectifs. target[0] est l'objectif du joueur 0, target[1] l'objectif du joueur 1
    u8    base[PLAYERNB - 2]    = { PLAYER_PERMUT0   }; // la base est donc l'ensemble des joueurs moins le joueur 0 et moins la target du joueur 0
    //u8    base[PLAYERNB - 2] = { 3,7,4,5,6,2 };       // la base est donc l'ensemble des joueurs moins le joueur 0 et moins la target du joueur 0
    // *
    // *
    // ***************************************************************************************************************************************
    // Vérifications de la cohérence de 'target' et  'base'
    for (u8 i = 0; i < PLAYERNB - 2; i++)
    {
        assert(base[i] != 0);               // 0 est toujours exclu de base !
        assert(base[i] != target[0]);       // target[0] est toujours exclu de base !
    }
    // Création des permutations
    u32 permutnb = factorial(PLAYERNB-2);
    u8* perm = (byte*)malloc(sizeof(u8) * permutnb * (PLAYERNB-2));
    u8* p = perm;
    permute(&p, base, 0, PLAYERNB - 2);
    p = perm;
    for (u32 i = 0; i < permutnb; i++, p += (PLAYERNB - 2))
    {
        printf("#%d: ", i);
        for (u8 j = 0; j < (PLAYERNB - 2); j++)
        {
            printf("%d", p[j]);
        }
        printf("\n");
    }
    printf("Soit un total de %d permutations.\n",permutnb);

    // ***************************************************************************************************************************************
    // *
    // Construction de la seconde ligne à partir d'une premiere ligne
    // *
    // ***************************************************************************************************************************************
    /*
    Node** ptrees = (Node**)malloc(sizeof(Node**) * permutnb);
    p = perm;
    Node** ppt = ptrees;
    u32 total = 0;
    for (u32 i = 0; i < permutnb; i++, ppt++, p += (PLAYERNB - 2))
    {
        *ppt = BuildLine1(p, target);
        total += (*ppt)->GetLeafNb(PLAYERNB - 2);
    }
    */
    // End Performance Timer
    QueryPerformanceCounter((LARGE_INTEGER*)&timeend);
    time = (f64)(timeend - timestart) * invfrequency;
    printf("%f sec \n", time);


    // DEUXIEME PASSE
    // Construction des premières "Lines"
    NodePile pile{ 2048 }; // L'objet Pile créé est local mais la mémoire allouée par Pile se fait via un malloc
    //Line** pplines = (Line**)malloc(sizeof(Line*) * permutnb);
    //Line** ppl = pplines;
    Line* pl0 = nullptr;
    Pile<Table> tables{ 1024 };
    u32 sz;
    u32 valide = 0;
    Table** ppt;
    p = perm;
    time = 0;
    totaltime = 0;
    lefttime = 0;
    FILE *pfile = nullptr;
    char filename[32];
    for (u32 i = 0; i < permutnb; i++,  p += (PLAYERNB - 2))
    {
        // On crée un fichier par permutation
        // Start Performance Timer 
        QueryPerformanceCounter((LARGE_INTEGER*)&timestart);

        pl0 = new Line(p, 0, target[0], nullptr);
        printf(" ----------------------------\n");
        printf(" Permutation #%d(%d/%d): \n", i, i+1, permutnb);
        pl0->Print();
        printf("\n");
        pl0->buildChildren(target, &pile);
        printf("%d different build lines.\n ", pl0->GetNbLeaf(PLAYERNB - 1));
        pl0->Clean();
        printf("%d different build lines AFTER CLEAN.\n ", pl0->GetNbLeaf(PLAYERNB - 1));

        if (pl0->m_pChild)
        {
            pl0->ExtractTables(&tables);
            ppt = tables.m_pile;
            sz = tables.size();
            for (u32 j = 0; j < sz; j++, ppt++)
            {
                if ((*ppt)->IsValid(PLAYERNB))
                {
                    if (!pfile)
                    {
                        sprintf(filename, "#%d.csv", i);
                        pfile = fopen(filename, "w");
                    }
                    valide ++;
                    printf(" ----------------------------\n");
                    (*ppt)->write(pfile);
                    (*ppt)->Print();
                    printf("\t TABLE VALIDE !!!! \n");
                    printf(" ----------------------------\n");
                }
                delete* ppt;
            }
        }
        tables.resize0();
        delete pl0;
        if (pfile)
        {
            fclose(pfile);
            pfile = nullptr;
        }

        printf(" %d TABLES VALIDEs!!!! \n",valide);

        // End Performance Timer
        QueryPerformanceCounter((LARGE_INTEGER*)&timeend);
        time = (f64)(timeend - timestart) * invfrequency;
        totaltime += time;
        lefttime = ((double)(permutnb - (i + 1)) / (double)(i + 1)) * totaltime;
        printf(" Process #%d/%d: %f sec \n", i + 1, permutnb,time);
        printf(" Temps Effectue : %f sec \n", totaltime);
        printf(" Temps Restant : %f sec \n", lefttime);
        printf(" Temps Total : %f sec \n", (totaltime + lefttime));

    }
    sprintf(filename, "_readme_%d_joueurs.txt",PLAYERNB);
    pfile = fopen(filename, "w");
    fprintf(pfile, "permutation initiale:");
    pl0 = new Line(perm, 0, target[0], nullptr);
    pl0->write(pfile);
    delete pl0;
    fprintf(pfile, "\n");
    fprintf(pfile, "cibles: ");
    for(u8 i=0;i<PLAYERNB;i++)
        fprintf(pfile, "%d,",target[i]);
    fprintf(pfile, " ( 0 a pour objectif %d,...)\n", target[0]);
    fprintf(pfile, "Durée traitement %f\n", totaltime);
    fprintf(pfile, "\n%d Solutions\n", valide);


    fclose(pfile);
    /*
   p = perm;
    ppt = ptrees;
    ppl = pplines;
    for (u32 i = 0; i < permutnb; i++, p += (PLAYERNB - 2), ppt++,ppl++)
    {
        printf("#%d:\tListe[0]: 0", i);
        for (u8 j = 0; j < (PLAYERNB - 2); j++)
        {
            printf("%d", p[j]);
        }

        printf("%d ", target[0]);
        (*ppl)->Print();
        printf(" --> %d\tListe[1] possibles.\n", (*ppt)->GetLeafNb(PLAYERNB - 2));
    }
    printf("---------------------------------------------------\n");
    printf("NOMBRE DE LISTE DE %d Cells presentes dans tous les arbres: %d \n", PLAYERNB - 1, total);

    printf("---------------------------------------------------\n");
    ptrees[0]->Print();

    printf("---------------------------------------------------\n");
    printf("---------------------------------------------------\n");

    ppl = pplines;
    for (u32 i = 0; i < permutnb; i++, ppl++)
    {
        if((*ppl)->m_pChild)
            (*ppl)->DeepPrint();
    }
    printf("**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**\n");
    printf("**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**\n");
    printf("**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**--**\n");
    pplines[62]->DeepPrint();
    */




    // END
    free(perm);
}
