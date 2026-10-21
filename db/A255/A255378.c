/*
 * A255378  Number of strings of n decimal digits that contain at least one
 *          string of exactly 8 consecutive "0" digits.
 * https://oeis.org/A255378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9*(10^(n-9) - a(n-9) + sum_{i=8}^{n-1} a(i)) for n>=9;
 *            initial values: a(0)=a(1)=...=a(7)=0, a(8)=1
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cached arrays and their capacities */
    static seq_t *A   = NULL;   /* a values   */
    static size_t  capA = 0;
    static seq_t *S   = NULL;   /* cumulative sums S[i] = sum_{k=8}^{i} a(k) */
    static size_t  capS = 0;
    static seq_t *pow10 = NULL; /* pow10[i] = 10^i  (i>=0) */
    static size_t  capP  = 0;
    static idx_t   max_n = 0;

    /* INITIALISE caches on the first call */
    if (capA == 0)
    {
        /* Allocate a little larger than needed for the first 9 indices */
        size_t initCap = 16;
        A = (seq_t *)malloc(initCap * sizeof(seq_t));
        S = (seq_t *)malloc(initCap * sizeof(seq_t));
        capA = capS = initCap;

        for (idx_t i = 0; i < 9; ++i)
        {
            if (i < 8)
                A[i] = (seq_t)0;
            else
                A[i] = (seq_t)1;

            if (i == 0)
                S[i] = (seq_t)0;
            else
                S[i] = S[i - 1] + A[i];
        }
        max_n = 8;   /* we have computed up to index 8 */

        /* initialise pow10[0] = 1 */
        pow10 = (seq_t *)malloc(sizeof(seq_t));
        pow10[0] = (seq_t)1;
        capP = 1;
    }

    /* enlarge pow10 if we need 10^(n-9) */
    if (n >= 9)
    {
        size_t needP = (size_t)(n - 9) + 1;   /* inclusive */
        if (capP < needP)
        {
            size_t newCap = capP ? capP << 1 : 1;
            while (newCap < needP) newCap <<= 1;
            seq_t *tmp = (seq_t *)realloc(pow10, newCap * sizeof(seq_t));
            pow10 = tmp;
            /* compute the new powers */
            for (size_t i = capP; i < newCap; ++i)
                pow10[i] = pow10[i - 1] * (seq_t)10;
            capP = newCap;
        }
    }

    /* if we already have the value, return it */
    if (n <= max_n)
        return A[n];

    /* enlarge A and S to hold up to index n */
    if (n + 1 > capA)
    {
        size_t newCap = capA ? capA << 1 : 1;
        while (newCap < (size_t)(n + 1)) newCap <<= 1;
        seq_t *tmpA = (seq_t *)realloc(A, newCap * sizeof(seq_t));
        seq_t *tmpS = (seq_t *)realloc(S, newCap * sizeof(seq_t));
        A = tmpA;
        S = tmpS;
        capA = capS = newCap;
    }

    /* compute new terms from max_n+1 up to n */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        /* i must be >= 9 because max_n was initialised to 8 */
        seq_t term = pow10[(size_t)(i - 9)];   /* 10^(i-9)   */
        term -= A[i - 9];                     /* subtract a(i-9)   */
        term += S[i - 1];                     /* add sum_{k=8}^{i-1} a(k)   */
        term *= (seq_t)9;                     /* multiply by 9   */
        A[i] = term;
        S[i] = S[i - 1] + A[i];
    }

    max_n = n;
    return A[n];
}
