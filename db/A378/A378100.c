/*
 * A378100  Number of involutions in the symmetric group S_n with at least one
 *          fixed point.
 * https://oeis.org/A378100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000085(n) – (n even ? (n‑1)!! : 0) where A000085(n) is
 *            the number of involutions in S_n. A000085 satisfies A(0)=1,
 *            A(1)=1, A(n)=A(n-1)+(n-1)*A(n-2) for n≥2. The sequence required
 *            by OEIS A378100 returns a(0)=0
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 31, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute the OEIS sequence A378100
   external linkage prescribed by the problem statement. */
seq_t a(idx_t n)
{
    /* a(0) = 0 by the definition of A378100 */
    if (n == 0)
        return (seq_t)0;

    /* static cache of A000085 values */
    static seq_t *afull = NULL;   /* array of A000085(n) values */
    static idx_t      cap = 0;    /* number of entries allocated  */

    /* Allocate or grow the cache up to index n. */
    if (cap <= n) {
        size_t newcap = (size_t)(n + 1);

        seq_t *tmp = (seq_t *)realloc(afull, newcap * sizeof(seq_t));
        if (!tmp)   /* allocation failure – return 0 as a safe fallback */
            return (seq_t)0;
        afull = tmp;

        /* initialise newly allocated part */
        if (cap == 0) { /* first time: put base cases */
            afull[0] = (seq_t)1;                 /* A000085(0) */
            if (newcap > 1)
                afull[1] = (seq_t)1;             /* A000085(1) */
            cap = newcap;
        } else { /* fill new entries using the recurrence */
            for (idx_t i = cap; i < newcap; ++i) {
                /* A000085(i) = A000085(i-1) + (i-1)*A000085(i-2) */
                seq_t prod = (seq_t)(i - 1) * afull[i - 2];
                afull[i]   = afull[i - 1] + prod;
            }
            cap = newcap;
        }
    }

    /* A000085(n) is now available */
    seq_t result = afull[n];

    /* Subtract the involutions with no fixed points when n is even */
    if ((n & 1) == 0) {
        seq_t sub = (seq_t)1;    /* (n‑1)!!  product of odd numbers < n */
        for (idx_t i = 1; i < n; i += 2) {
            sub *= (seq_t)i;
        }
        result -= sub;
    }

    return result;
}
