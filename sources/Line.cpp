
#include "Node.h"
#include "NodePile.h"
#include "Table.h"
#include "Line.h"


Line::Line(Line* parent)
{
    m_pChild = nullptr;
    m_pParent = parent;
    /*
    memset(m_cell, 0, sizeof(u8) * PLAYERNB);
    memset(m_PinLine, 0, sizeof(u8) * PLAYERNB);
    memset(m_chunkL, 0, sizeof(u16) * PLAYERNB);
    memset(m_chunkR, 0, sizeof(u16) * PLAYERNB);
    memset(m_chunkX, 0, sizeof(u16) * PLAYERNB);
    */
    if (parent)
    {
        m_pNxt = m_pParent->m_pChild;
        m_pParent->m_pChild = this;
        m_deep = m_pParent->m_deep + 1;
    }
    else
    {
        m_pNxt = nullptr;
        m_deep = 0;
    }
}

Line::Line(const u8* pfull, Line *parent )
{
	m_pChild = nullptr;
	m_pParent	= parent;
    memcpy(m_cell, pfull, sizeof(u8) * PLAYERNB);
    setupChunksAndPinLine();

    if (parent)
	{
		m_pNxt = m_pParent->m_pChild;
		m_pParent->m_pChild = this;
        m_deep = m_pParent->m_deep + 1;
	}
	else
	{
		m_pNxt = nullptr;
        m_deep = 0;
	}
}

Line::Line(const u8* pmid, const u8 first, const u8 target, Line *parent)
{
 //   init chunk et posinline dan sles constructeurs et buildlines via une fonction 'setupChunksAndPinLine'
	
    m_pChild = nullptr;
	m_pParent = parent;
	if (parent)
	{
		m_pNxt = m_pParent->m_pChild;
		m_pParent->m_pChild = this;
        m_deep = m_pParent->m_deep + 1;
	}
	else
	{
		m_pNxt = nullptr;
        m_deep = 0;
	}

	m_cell[0] = first;
	m_cell[PLAYERNB - 1] = target;
	memcpy(&m_cell[1], pmid, sizeof(u8) * (PLAYERNB-2));
    setupChunksAndPinLine();
}

Line::~Line()
{
	if (m_pChild)
		delete(m_pChild);
	if (m_pNxt)
		delete(m_pNxt);
}


void Line::Remove()
{
    assert(m_pParent);

    Line* pc = m_pParent->m_pChild;
    if (pc == this)
    {
        m_pParent->m_pChild = m_pNxt;
    }
    else
    {
        assert(pc->m_pNxt);
        while (pc->m_pNxt != this) { pc = pc->m_pNxt; }
        assert(pc->m_pNxt == this);
        pc->m_pNxt = m_pNxt;
    }

    m_pParent = nullptr;
    m_pNxt = nullptr;
    // memset(m_chunkL, 0, sizeof(u16) * PLAYERNB); restent valides
    // memset(m_chunkR, 0, sizeof(u16) * PLAYERNB); restent valides
    // plus de parent il faut rebuild m_chunkX ... TODO sauf si le sort de la ligne est la suppression ... 
    /*
    for (u8 i = 0; i < PLAYERNB; i++)
    {
        m_chunkX[i] = m_chunkL[i] | m_chunkR[i];
    }
    */
}

u32 Line::GetChildrenNb()
{
    if (m_pChild)
    {
        u32 nb = 0;
        Line* pl = m_pChild;
        while (pl)
        {
            nb++;
            pl = pl->m_pNxt;
        }
        return nb;
    }
    else
    {
        return 0;
    }
}

void Line::write(FILE* pfile)
{
    for (u8 j = 0; j < PLAYERNB; j++)
    {
        fprintf(pfile,"%d;", m_cell[j]);
    }
}

void Line::read(FILE* pfile)
{
    //TO DO fgets
}

void Line::Print()
{
    for (u8 j = 0; j < PLAYERNB; j++)
    {
        printf("%d;", m_cell[j]);
    }
}

void Line::DeepPrint()
{
    for (u8 i = 0; i <m_cell[0]; i++)
       printf("\t");

    Print();
    printf("\n");
    // Mon fils
    if (m_pChild)
        m_pChild->DeepPrint();

    // mon frere
    if (m_pNxt)
        m_pNxt->DeepPrint();
}
static void _printChunk(const u16 chunk)
{
    for (s8 i = PLAYERNB-1; i >= 0; i--)
    {
        printf("%d", (chunk >> i) & 1);
    }
}
void Line::setupChunksAndPinLine()
{
    // POSITIONS des joueurs sur cette ligne en accès inverse
    // ... ( à quelle position est le joueur i ? )
    for (u8 i = 0; i < PLAYERNB; i++)
        m_PinLine[m_cell[i]] = i;
    // ETUDE de CETTE LIGNE et definition des "chunk" pour chaque joueur
     // Les chunks sont des champs de bits où chaque bit représente un joueur, le BIT_0 pour le joueur 0, le BIT_1 le joueur 1, etc ... 
     // m_chunkL     Sur le m_chunkL[i] sont set à 1 tous les joueurs à Gauche ( Left ) du joueur i.
     //              
     // m_chunkR     Sur le m_chunkR[i] est set à 1 le joueur immédiatement à droite ( Right ) du joueur i. m_chunkR[i] n'a donc qu'un seul bit à 1 ( ou aucun si le joueur est le dernier )
     //
     // m_chunkX     Le chunkX[i] est m_chunkL[i] | m_chunkR[i] le tout & avec le chunkX[i] du parent. 
     //              Il représente donc les joueurs qui sont sur cette ligne et toutes les précédentes toujours avant ou juste après i.
     //              le & ne garde que les joueurs présents sur toutes les lignes précédentes jusqu'à celles-ci ( précédentes dan sle sens lignes parentes )

     //u16 chunk[PLAYERNB];
    for (u8 i = 0; i < PLAYERNB; i++)
    {
        // on set à 1 le bit du joueur situé juste après le joueur i sur la ligne 0
        if (m_PinLine[i] < PLAYERNB - 1)
            m_chunkR[i] = 1 << m_cell[m_PinLine[i] + 1];
        // sauf pour le joueur i situé sur la dernière cellule de la ligne ( et qui est donc l'objectif )
        else
        {
            assert(i == m_cell[PLAYERNB - 1]); // on vérifie que i est bien l'objectif de la ligne 0
            m_chunkR[i] = 0;
        }
        // on set à 1 le bit n de chaque joueur (n = line[j], joueur situé en position j sur la ligne )
        for (u8 j = 0; j < m_PinLine[i]; j++)
            m_chunkL[i] |= 1 << m_cell[j];

        assert(!((m_chunkL[i] >> i) & 1));
        assert(!((m_chunkR[i] >> i) & 1));

        // pack avec les chunks du parent.
        if (m_pParent)
        {
            m_chunkX[i] = (m_chunkL[i] | m_chunkR[i]) & m_pParent->m_chunkX[i];
           // _printChunk(m_chunkX[i]);
           // printf("\n");
        }
        else
        {
            m_chunkX[i] = m_chunkL[i] | m_chunkR[i];
           // _printChunk(m_chunkX[i]);
           // printf("\n");
        }
    }
}

bool Line::IsValid()
{
    // Test à la volée utilisé juste après la création de la ligne pour potentiellement l'éliminer de suite.
    // Ce test est le même que le IsValid de la class Table mais appliqué à une table incomplète...
    if (m_pParent)
    {
        Table table;
        table.m_pLine[0] = this;
        u8 i = 1;
        Line* p = m_pParent;
        do
        {
            table.m_pLine[i] = p;
            i++;
            p = p->m_pParent;
        } while (p);

        return table.IsValid(i);
    }

    return true;
}




void Line::buildChildren(u8* ptarget, NodePile* pile)
{
    assert(pile->isEmpty());
    assert(m_cell[PLAYERNB - 1] == ptarget[m_cell[0]]);
    
    Line*pl,*pl2;

    // DEFINITION du joueur en début de la prochaine série de lignes.
    // METHODE 1
    /*    
    u8 first = m_cell[0] + 1;

    if (first == PLAYERNB)
        return;
    */
    //METHODE 2
    // on choisi comme premier joueur celui qui sera le plus restrictif très vite...
    // a) construction d'un champ de bit representant les joueurs occupant deja la premiere place sur cette "branche" ( line->parent->...->parent) de lignes
   
    u16 already_first = 0;
    pl = this;
    while (pl)
    {
        already_first |= 1 << pl->m_cell[0];
        pl = pl->m_pParent;
    }
    if ((already_first & MASK_ALLPLAYERS) == MASK_ALLPLAYERS)
        return;
    // b) on choisi comme prochain "first" celui qui ,sur toutes les lignes de cette branche est le plus à gauche parmis les joueurs n'ayant pas encore été first
    u8 first = 255;
    for (u8 i = 0; (i < PLAYERNB)&&(first == 255); i++)
    {
        pl = this;
        while (pl)
        {
            if (!((already_first >> pl->m_cell[i]) & 1))
            {
                first = pl->m_cell[i];
                //printf("First : Player %d\n", first);
                break;
            }

            pl = pl->m_pParent;
        }
    }

    assert(first != 255);
    
/*    // POSITIONS des joueurs sur cette ligne en accès inverse
    // ... ( à quelle position est le joueur i ? )
    u8 posinline[PLAYERNB];
    for (u8 i = 0; i < PLAYERNB; i++)
        posinline[m_cell[i]] = i;   
*/
/*
    // ETUDE de CETTE LIGNE et definition des "chunk" pour chaque joueur
    // Les chunks sont des champs de bits où chaque bit représente un joueur, le BIT_0 pour le joueur 0, le BIT_1 le joueur 1, etc ... 
    // m_chunkL     Sur le m_chunkL[i] sont set à 1 tous les joueurs à Gauche ( Left ) du joueur i.
    //              
    // m_chunkR     Sur le m_chunkR[i] est set à 1 le joueur immédiatement à droite ( Right ) du joueur i. m_chunkR[i] n'a donc qu'un seul bit à 1 ( ou aucun si le joueur est le dernier )
    //
    // m_chunkX     Le chunkX[i] est m_chunkL[i] | m_chunkR[i] le tout & avec le chunkX[i] du parent. 
    //              Il représente donc les joueurs qui sont sur cette ligne et toutes les précédentes toujours avant ou juste après i.
    //              le & ne garde que les joueurs présents sur toutes les lignes précédentes jusqu'à celles-ci ( précédentes dan sle sens lignes parentes )

    //u16 chunk[PLAYERNB];
    for (u8 i = 0; i < PLAYERNB; i++)
    {
        // on set à 1 le bit du joueur situé juste après le joueur i sur la ligne 0
        if (posinline[i] < PLAYERNB - 1)
            m_chunkR[i] = 1 << m_cell[posinline[i] + 1];
        // sauf pour le joueur i situé sur la dernière cellule de la ligne ( et qui est donc l'objectif )
        else
        {
            assert(i == m_cell[PLAYERNB - 1]); // on vérifie que i est bien l'objectif de la ligne 0
            m_chunkR[i] = 0;
        }
        // on set à 1 le bit n de chaque joueur (n = line[j], joueur situé en position j sur la ligne )
        for (u8 j = 0; j < posinline[i]; j++)
            m_chunkL[i] |= 1 << m_cell[j];

        assert(!((m_chunkL[i] >> i) & 1));
        assert(!((m_chunkR[i] >> i) & 1));

        // pack avec les chunks du parent.
        if (m_pParent)
        {
            m_chunkX[i] = (m_chunkL[i] | m_chunkR[i]) & m_pParent->m_chunkX[i];
        }
        else
        {
            m_chunkX[i] = m_chunkL[i] | m_chunkR[i];
        }
    }
*/

    // CONSTRUCTIONS DE TOUTES LES VERSIONS POSSIBLES DE LA LIGNE commencant par first et se terminant par target[first]
    // sauf forme d'un arbre où chaque noeud est une cellule de la ligne. chaque ligne comptant PLAYERNB cellules 
    // la chaine pere fils la plus longue comptera donc ... PLAYERNB noeud... en fait non, seulement PLAYERNB-1 noeud ...
    // car on insère pas dans l'arbre le dernier noeud car c'est toujours le même, c'est target[first]. Ce choix est fait pour économiser de la mémoire.
    // first aussi est toujours pareil mais on l'insère comme noeud racine, il est unique.
    u8 objectif = ptarget[first];
    Node* pn0 = new Node(first);
    Node* pn;
    u16 hint, insert;
    u8 p;

    u16 pn_chunkL,pn_chunkR,cancel;
    u16 insert2;
    // On push ce noeud sur la pile et Go
    pile->push(pn0);
    while (!pile->isEmpty())
    {
        pn = pile->pop();
        // rechecherche pn0->id dans line0_chunk
        hint = m_chunkX[pn->m_Id] & pn->m_isDead;
        assert(!((hint >> pn->m_Id) & 1)); //  le bit m_Id est à 1 dans m_isDead, cependant il est (normalement) à 0 dans lchunk donc à 0 dans hint !
        // Si il y a une intersection entre lchunk et isDead, alors...
        if (hint)
        {
            // on insère tout le monde sauf les ancetres, sois même et l'objectif courant
            insert = ~(pn->m_isDead | (1 << objectif));
        }
        // Sinon, si on ne détecte aucune intersection
        else
        {
            // on insère chunk
            insert = m_chunkX[pn->m_Id] & ~(1 << objectif);
        }
        /*
        // Nouveau test
        pn_chunkL = pn->m_isDead | (~(1 << pn->m_Id));
        pn_chunkR = 0;
        cancel = 0;
        for (p = 0; p < PLAYERNB; p++)
        {
            if ((insert >> p) & 1)
            {
                pn_chunkR = 1 << p;
                pl = this;
                while (pl)
                {
                    // si posiiton du jour sur ligne precedente > position du joueur sur ligne en construction
                    if (pl->m_PinLine[pn->m_Id] > pn->m_rang)
                    {
                        // si position du joueur sur ligne en construction non nulle ( n'est pas en debut de ligne )
                        if (pn->m_rang)
                        {
                            // Si les morts avant joueur sur ligne en construction sont tous morts egalement avant joueur sur ligne precedente
                            if ((pn_chunkL & pl->m_chunkL[pn->m_Id]) == pn_chunkL)
                                // Si prochain mort sur ligne en construction est mort avant ou juste après le joueur sur ligne precedente...
                                if ((pn_chunkR & (pl->m_chunkL[pn->m_Id] | pl->m_chunkR[pn->m_Id]))==0)
                                {
                                    cancel|=pn_chunkR;
                                    break;
                                }
                        }
                    }
                    else
                    {
                        if (pl->m_PinLine[pn->m_Id])
                        {

                            if ((pn_chunkL & pl->m_chunkL[pn->m_Id]) == pl->m_chunkL[pn->m_Id])
                                if ((pl->m_chunkR[pn->m_Id] & (pn_chunkL | pn_chunkR))==0)
                                {
                                    cancel|=pn_chunkR;
                                    break;
                                }
                        }
                    }

                    pl = pl->m_pParent;
                }
            }
        }
        insert &= ~cancel;
        */
        // on insère comme joueur possible tous les joueurs dont les bits sont à 1 dans insert
        // ... et on "empile" les nodes créés sur la pile récursive explicite
        for (p = 0; p < PLAYERNB; p++)
        {
            if ((insert >> p) & 1)
            {
                pile->push(new Node(p, pn));
            }
        }
    }
   // printf("Ligne #%d : %d versions\n", first, pn0->GetLeafNb(PLAYERNB - 2));
    
    // CONSTRUCTIONS DE TOUTES LES VERSIONS POSSIBLES DE LA LIGNE .. suite et fin.
    // à partir du noeud racine ( la cellule contenant first ) on parcourt l'arbre des cellules à la recherche de toute les chaines complètes
    // c'est à dire toute  les chaines pere/fils de la bonne longueur ( PLAYERNB-1 ) et on construit la ligne qui en découle.
    // Toutes les lignes ainsi construites seront enfin de 'this' ( cette ligne )
    u32 n = pn0->BuildLines(this, objectif);
    delete(pn0);
    //printf("[ligne %d]:: %d versions \n", first, n);

    // ON AVANCE A "LA" LIGNE SUIVANTE ( et toutes ses versions possibles )
    // pour chacune des lignes enfant de this qu'on vient de construire (qui sont donc toutes filles de this et donc toutes soeurs) ...
    // ... on rappelle cette même fonction.
    // Ainsi chacune de ses 'filles' deviendra à son tour 'mère' de x lignes filles, chacune d'elles représentant une version possible de la ligne suivante ...
    // et ainsi de suite jusqu'à ce que la bonne profondeur soit atteinte, c'est à dire jusqu'à ce que m_cell[0] = PLAYERNB-1.

    pl = m_pChild;
    while (pl)
    {
        pl->buildChildren(ptarget, pile);
        pl = pl->m_pNxt;
    }

}

u32 Line::GetNbLeaf(const u8 deep)
{
    u32 nb = 0;
    // ce node est une extrémité on remonte jusqu'en haut pour construire
    if (m_deep == deep && (!m_pChild) )
        nb = 1;
    // mon frere
    if (m_pNxt)
        nb += m_pNxt->GetNbLeaf(deep);
    // Mon fils
    if (m_pChild)
        nb += m_pChild->GetNbLeaf(deep);

    return nb;

}

u32 Line::GetNbLeaf()
{
    u32 nb = 0;
    // ce node est une extrémité on remonte jusqu'en haut pour construire
    if (!m_pChild)
        nb = 1;
    // mon frere
    if (m_pNxt)
        nb += m_pNxt->GetNbLeaf();
    // Mon fils
    if (m_pChild)
        nb += m_pChild->GetNbLeaf();

    return nb;
}

void Line::Clean()
{
    if (m_pChild)
        m_pChild->Clean();

    if (m_pNxt)
        m_pNxt->Clean();

    // Est une extrémité ?
    if ( (!m_pChild) && (m_deep < (PLAYERNB - 1)) )
    {
        if (m_pParent)
        {
            Remove();
            delete this;
        }
        #ifdef DEBUG
        else
            assert(!m_pNxt);
        #endif
    }
}

u32 Line::ExtractTables(Pile<Table>* ppile)
{
    u32 nb = 0;
    // cette ligne est une extrémité on remonte jusqu'en haut pour construire la table
    if (m_deep == (PLAYERNB - 1))
    {
        assert(!m_pChild);

        Table* ptb = new Table();
        ppile->push(ptb);
        Line** ppl = &ptb->m_pLine[PLAYERNB - 1];
        Line* pl = this;
        while (pl)
        {
            *ppl = pl;
            ppl--;
            pl = pl->m_pParent;
        }
        nb = 1;
    }
    // mon frere
    if (m_pNxt)
        nb += m_pNxt->ExtractTables(ppile);
    // Mon fils
    if (m_pChild)
        nb += m_pChild->ExtractTables(ppile);

    return nb;
}
