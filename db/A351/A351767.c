/*
 * A351767  Expansion of e.g.f. exp( x/(1-x)^3 ) / (1-x)^3.
 * https://oeis.org/A351767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n*a(n‑1) - (n‑1)*(6*n‑5)*a(n‑2) * +
 *            (n‑2)*(n‑1)*(4*n‑3)*a(n‑3) * - (n‑3)*(n‑2)*(n‑1)²*a(n‑4) *
 *            Initial values: a(0)=1, a(1)=4, a(2)=25, a(3)=214
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: holds computed terms up to the highest index requested */
    static seq_t *cache     = NULL;
    static size_t  capacity = 0;   /* allocated array size */
    static idx_t   next = 0;       /* next index to compute */

    /* grow cache if needed */
    if (capacity <= n) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache     = tmp;
        capacity  = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = next; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 4;
        } else if (i == 2) {
            cache[2] = 25;
        } else if (i == 3) {
            cache[3] = 214;
        } else {
            seq_t term1 = ((seq_t)4 * (seq_t)i) * cache[i-1];
            seq_t term2 = (seq_t)(i-1) *
                          (seq_t)(6*i - 5) * cache[i-2];
            seq_t term3 = (seq_t)(i-2) *
                          (seq_t)(i-1) *
                          (seq_t)(4*i - 3) * cache[i-3];
            seq_t term4 = (seq_t)(i-3) *
                          (seq_t)(i-2) *
                          (seq_t)(i-1) *
                          (seq_t)(i-1) * cache[i-4];
            cache[i] = term1 - term2 + term3 - term4;
        }
    }

    /* update next index to avoid recomputing what we already did */
    next = n + 1;

    return cache[n];
}
