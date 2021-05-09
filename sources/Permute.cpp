#include "TFJM.h"

int Total = 0;

/* Function to swap values at two pointers */
static void swap(byte*px, byte*py)
{
    byte tmp;
    tmp = *px;
    *px = *py;
    *py = tmp;
}
/* Function to print permutations of string
This function takes three parameters:
1. String
2. Starting index of the string
3. Ending index of the string. */
void permute(byte**ppr, byte* a, const byte l, const byte sz)
{
#ifdef _DEBUG
    static unsigned long    Count;
#endif
    if (l == sz - 1)
    {
#ifdef _DEBUG
        Count += 1;
        printf(" Permutation #%d : ", Count);
        printf("%d %d %d %d %d %d %d \n", a[0], a[1], a[2], a[3], a[4], a[5], a[6]);
#endif
        memcpy( *ppr, a, sz*sizeof(byte));
        *ppr += sz;
    }
    else
    {
        for (byte i = l; i < sz; i++)
        {
            swap((a + l), (a + i));
            permute(ppr, a, l + 1, sz);
            swap((a + l), (a + i)); //backtrack
        }
    }
}



/*
u32 swapu32(u32 v, byte f, byte l)
{
    // mask a: (0xF<<(a<<2))
    // mask b: (0xF<<(b<<2))
    byte p0 = v & 0xF;
    byte p1 = (v >> 4) & 0xF;
    byte p2 = (v >> 8) & 0xF;
    byte p3 = (v >> 12) & 0xF;
    byte p4 = (v >> 16) & 0xF;
    byte p5 = (v >> 20) & 0xF;
    byte p6 = (v >> 24) & 0xF;
    byte p7 = (v >> 28) & 0xF;

    u32 msk = ((0xF << (f << 2)) | (0xF << (l << 2)));
    u32 sw = (v & (~msk)) | (((v >> (l << 2))&0xF)<<(f<<2)) | (((v >> (f << 2))&0xF)<<(l<<2));

   p0 = sw & 0xF;
   p1 = (sw >> 4) & 0xF;
   p2 = (sw >> 8) & 0xF;
   p3 = (sw >> 12) & 0xF;
   p4 = (sw >> 16) & 0xF;
   p5 = (sw >> 20) & 0xF;
   p6 = (sw >> 24) & 0xF;
   p7 = (sw >> 28) & 0xF;

    return  sw;

}
*/

#define SWAP(v,f,l)     ((v & (~((0xF << (f << 2)) | (0xF << (l << 2))))) | (((v >> (l << 2))&0xF)<<(f<<2)) | (((v >> (f << 2))&0xF)<<(l<<2)))

u32* permuteu32(u32*pr, u32 *pa, const byte l, const byte sz)
{
#ifdef _DEBUG
    static unsigned long    Count;
#endif
    if (l == sz - 1)
    {
#ifdef _DEBUG
        Count += 1;

        printf(" Permutation #%d : ", Count);
        for(byte j = 0; j<8;j++)
            printf("%d", (*pa >> (j<<2))&(0xF));
        printf("\n");
#endif
        * pr = *pa;
        pr++;

    }
    else
    {
        for (byte i = l; i < sz; i++)
        {
            //*pa = swapu32(*pa,l,i);
            *pa = SWAP(*pa, l, i);
            pr = permuteu32(pr, pa, l + 1, sz);
            //*pa = swapu32(*pa,l,i); //backtrack
            *pa = SWAP(*pa, l, i);
        }
    }
    return pr;
}
