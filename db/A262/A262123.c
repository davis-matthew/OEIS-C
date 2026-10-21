/*
 * A262123  a(1) + a(2) + ... + a(n) is the representation as a sum of n
 *          squares of the smallest integer needing n squares (using the
 *          greedy algorithm).
 * https://oeis.org/A262123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=a(3)=1, a(4)=4; for n>=4: a(n+1) = (a(n)/2 +
 *            sqrt(a(n)))^2
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the greedy decomposition of the smallest integer
   that needs exactly n squares.  Each a(n) is itself a perfect square. */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *seq = NULL;
    static idx_t cap = 0;       /* current capacity of seq  */
    static idx_t last = 0;      /* highest index that has been computed */

    if (n == 0) return (seq_t)0;   /* undefined for n==0, harmless */

    /* Ensure the array is large enough */
    if (n > cap)
    {
        idx_t newcap = cap ? cap * 2 : 16;  /* start with some capacity */
        while (newcap < n) newcap *= 2;
        seq = (seq_t*)realloc(seq, newcap * sizeof(seq_t));
        if (!seq) exit(1);   /* allocation failed */
        cap = newcap;
    }

    /* Handle small indices directly */
    switch (n)
    {
        case 1: return (seq_t)1;
        case 2: return (seq_t)1;
        case 3: return (seq_t)1;
        case 4: return (seq_t)4;
    }

    /* Initialise the cached values if this is the first time */
    if (last < 4)
    {
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)1;
        seq[3] = (seq_t)1;
        seq[4] = (seq_t)4;
        last = 4;
    }

    /* Compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t prev   = seq[i - 1];                     /* a(i-1)   */
        seq_t sqroot = floor_sqrt(prev);               /* sqrt(a(i-1))  */
        seq_t base   = (prev / (seq_t)2) + sqroot;     /* a(i-1)/2 + sqrt(a(i-1)) */
        seq[i] = base * base;                          /* square the result */
    }

    last = n;
    return seq[n];
}
