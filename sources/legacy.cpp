// #include "TFJM.h"

/*
PYTHON CODE
def check(tablePref, objectifTable, listeCandidat) :

    #nbrLine = len(tablePref)

    for candidat in listeCandidat :
        currentDead = candidat
        candidatStillAlive = [0] * nbrCandidats
        candidatStillAlive[currentDead] = -1

        for j in range(nbrCandidats - 2) :

            if (candidatStillAlive[tablePref[0][currentDead]] != -1) :
                currentDead = tablePref[0][currentDead]
                candidatStillAlive[currentDead] = -1

            elif(candidatStillAlive[tablePref[1][currentDead]] != -1) :
                currentDead = tablePref[1][currentDead]
                candidatStillAlive[currentDead] = -1

            elif(candidatStillAlive[tablePref[2][currentDead]] != -1) :
                currentDead = tablePref[2][currentDead]
                candidatStillAlive[currentDead] = -1

            else:
                return False

    if (objectifTable[candidat] == currentDead) :
        return False

        return True
*/
/*
typedef KillTable   KillTable;
struct KillTable
{
    byte** pptr[8];
};

#define ISDEAD(dead,n)      ((dead>>n)&1)
#define KILL(dead,n)        (dead|(1<<n))
int check( const byte *permut, KillTable* pkillist, const byte* pobjectiflist, const byte sz  )
{
    u32 dead;
    byte currentdead;
    byte candidat, j;
    for (candidat = 0; candidat < sz; candidat++)
    {
        currentdead = candidat;
        dead = 1<<candidat;

        for (j = 0; j < sz - 2; j++)
        {
            if (!ISDEAD(dead, (*pkillist->pptr[0])[currentdead]))
            {
                currentdead = (*pkillist->pptr[0])[currentdead];
                dead = KILL(dead, (*pkillist->pptr[0])[currentdead]);
                
                if (ISDEAD(dead, pobjectiflist[candidat]))
                    return 0;
            }
            else 
            {

                break;
            }
            
        }
//        if (ISDEAD(dead, pobjectiflist[candidat]))
    }
    return 0;
}


int main()
{

    u64			timestart, timeend, frequency;
    f64			time, invfrequency;
    QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
    invfrequency = (f64)1000 / frequency;


/ *
    byte base[7] = { 0,1,2,3,4,5,6 };
    byte*perm = (byte*)malloc(sizeof(byte)*5040*7);
    byte* p = perm;
    permute(&p,base,0,7);
    p = perm;
    for (int i = 0; i < 5040; i++, p += 7)
    {
        printf("%d %d %d %d %d %d %d \n", p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
    }
    free(perm);
* /
    u32  bu32 = 0 | (1 << 4) | (2 << 8) | (3 << 12) | (4 << 16) | (5 << 20) | (6 << 24) | (7 << 28);
    u32 *pu32 = (u32*)malloc(sizeof(u32)*5040);

    byte base[7] = { 0,1,2,3,4,5,6 };
    byte* perm = (byte*)malloc(sizeof(byte) * 5040 * 7);
    byte* pb = perm;

    QueryPerformanceCounter((LARGE_INTEGER*)&timestart);
    u32 iter = 100000;
    for (u32 a = 0; a < iter; a++)
    {
        //pb = perm;
        //permute(&pb, base, 0, 7);
        permuteu32(pu32, &bu32, 0, 7);
    }
    QueryPerformanceCounter((LARGE_INTEGER*)&timeend);
    time = (f64)(timeend - timestart) * invfrequency;
    printf("%f millisec for %d iterations \n", time);
    printf("-----------------------------------------------------------------\n");
    printf("%f millisec for One\n", time/(f64)iter);
    
    permuteu32(pu32, &bu32, 0, 7);
    u32* p = pu32;
    for (u32 a = 0; a < 5040; a++,p++)
    {
        printf(" Permutation #%d : ", a);
        for (byte j = 0; j < 7; j++)
            printf("%d", (*p >> (j << 2)) & (0xF));
        printf("\n");
    }



    free(pu32);
    free(perm);
}
*/
/*
nbrCandidats = 7
objectifTable = [1, 2, 0, 4, 5, 6, 3]
listeCandidat = [i for i in range(nbrCandidats)]


*/




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
