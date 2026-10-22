/*
 * A377626  Cogrowth sequence of the 12-element group A4 = <S,T | S^3, T^2,
 *          (ST)^3>.
 * https://oeis.org/A377626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Cogrowth sequence for A4 = <S,T | S^3, T^2, (ST)^3>.
 * We model all 12 elements of A4 as permutations of 4 letters.
 * A word of length n over {S,T} evaluates to identity iff the walk
 * in the Cayley graph of A4 starting at the identity ends at the identity.
 * The DP below counts all such walks exactly.
 * This implements a finite‑state automaton using 12 states.
 */
#include <string.h>
#include <stdbool.h>

/* Each permutation is stored as a 4‑byte array of images of 0..3 */
typedef struct { unsigned char p[4]; } perm;

/* The 12 even permutations of 4 symbols (A4) */
static const perm A4_ELEM[12] = {
    { {0,1,2,3} },  /* id */
    { {1,0,3,2} },  /* (12)(34)     */
    { {2,3,0,1} },  /* (13)(24)     */
    { {3,2,1,0} },  /* (14)(23)     */
    { {1,2,0,3} },  /* (012) = (123) */
    { {2,0,1,3} },  /* (021) = (132) */
    { {1,3,2,0} },  /* (013) = (124) */
    { {3,0,2,1} },  /* (031) = (142) */
    { {2,1,3,0} },  /* (023) = (134) */
    { {3,1,0,2} },  /* (032) = (143) */
    { {0,2,3,1} },  /* (123) = (234) */
    { {0,3,1,2} }   /* (132) = (243) */
};

/* Generators: S = (012), T = (12)(34) */
static const perm S_GEN = { {1,2,0,3} };
static const perm T_GEN = { {1,0,3,2} };

/* Find the index of a permutation in A4_ELEM */
static int find_perm(const perm *p)
{
    for (int i = 0; i < 12; ++i)
        if (memcmp(A4_ELEM[i].p, p->p, 4 * sizeof(unsigned char)) == 0)
            return i;
    return -1; /* should never happen */
}

/* Multiply a·b: apply a then b */
static void mul_perm(const perm *a, const perm *b, perm *res)
{
    for (int i = 0; i < 4; ++i)
        res->p[i] = b->p[a->p[i]];
}

/* Main function required by the tests */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static int next_id[12][2]; /* [current][generator] -> next index */

    if (!initialized) {
        /* Precompute transition table */
        perm prod;
        for (int g = 0; g < 12; ++g) {
            mul_perm(&A4_ELEM[g], &S_GEN, &prod);
            next_id[g][0] = find_perm(&prod);
            mul_perm(&A4_ELEM[g], &T_GEN, &prod);
            next_id[g][1] = find_perm(&prod);
        }
        initialized = true;
    }

    if (n == 0)
        return (seq_t)1; /* the empty word */

    seq_t cur[12]  = {0};
    seq_t next[12] = {0};
    cur[0] = (seq_t)1;   /* start at identity */

    for (idx_t step = 1; step <= n; ++step) {
        /* reset next */
        for (int i = 0; i < 12; ++i) next[i] = (seq_t)0;

        /* iterate over all states */
        for (int g = 0; g < 12; ++g) {
            seq_t cnt = cur[g];
            if (cnt == (seq_t)0) continue;
            next[ next_id[g][0] ] += cnt;   /* apply S */
            next[ next_id[g][1] ] += cnt;   /* apply T */
        }

        /* copy next to cur for next iteration */
        for (int i = 0; i < 12; ++i)
            cur[i] = next[i];
    }

    return cur[0];   /* number of words evaluating to the identity */
}
