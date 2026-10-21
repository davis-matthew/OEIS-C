/*
 * A293366  Number of partitions of n where each part i is marked with a word
 *          of length i over a binary alphabet whose letters appear in
 *          alphabetical order and both letters occur at least once in the
 *          partition.
 * https://oeis.org/A293366
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{i≥1} 1/(1-(i+1)q^i) minus twice
 *            the coefficient of q^n in ∏_{i≥1} 1/(1-q^i)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute weighted partition numbers and ordinary partition numbers
   incrementally, caching the full table up to the largest n requested. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 2; return 0 for smaller indices. */
    if (n < 2)
        return 0;

    /* Static state: entire tables for weighted and ordinary partitions
       up to the highest n seen so far. */
    static seq_t *dpG = NULL;   /* weighted partitions */
    static seq_t *dpP = NULL;   /* ordinary partitions  */
    static idx_t  maxComputed = 0;

    /* If a larger n is requested, recompute the entire tables up to n. */
    if (n > maxComputed) {
        /* Allocate fresh tables initialized to zero. */
        seq_t *newG = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        seq_t *newP = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!newG || !newP) {   /* allocation failure – return 0 (unlikely). */
            if (newG) free(newG);
            if (newP) free(newP);
            return 0;
        }

        /* Base case: empty partition. */
        newG[0] = newP[0] = 1;

        /* Standard coin‑change DP for weighted and ordinary partitions. */
        for (idx_t i = 1; i <= n; ++i) {
            seq_t weight = (seq_t)(i + 1);            /* (i+1) choices for part i */
            for (idx_t s = i; s <= n; ++s) {
                newG[s] += newG[s - i] * weight;       /* weighted part */
                newP[s] += newP[s - i];                /* ordinary part */
            }
        }

        /* Free old tables and update static references. */
        if (dpG) free(dpG);
        if (dpP) free(dpP);
        dpG = newG;
        dpP = newP;
        maxComputed = n;
    }

    /* a(n) = G(n) – 2·P(n)  */
    return dpG[n] - (seq_t)2 * dpP[n];
}
