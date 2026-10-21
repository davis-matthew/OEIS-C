/*
 * A284864  Row sums of Sheffer triangle S2[3,2] given by A225466.
 * https://oeis.org/A284864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented:
   a(0) = 1
   a(n) = 2*a(n-1) + Sum_{k=1..n} binom(n-1,k-1) * 3^k * a(n-k)
   (OEIS entry A284864) */

#include <stdlib.h>
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Cache of already computed terms. */
    static seq_t *cache  = NULL;   /* dynamic array of seq_t */
    static size_t len   = 0;       /* number of terms stored */
    static size_t cap   = 0;       /* current allocation capacity */

    /* Allocate initial cache if required */
    if (cache == NULL) {
        /* Start with a small capacity to avoid frequent reallocs */
        cap = (n >= 64 ? n + 1 : 64);
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();
    }

    /* Grow cache if n exceeds current capacity */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise first term if cache is empty */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;   /* safety */
            continue;
        }

        seq_t sum  = (seq_t)0;
        seq_t binom = (seq_t)1;    /* binom(i-1,0) */
        seq_t pow3  = (seq_t)3;    /* 3^1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* term = binom * 3^k * a(i-k) */
            seq_t term = binom * pow3 * cache[i - k];
            sum += term;

            if (k < i) {
                /* Update binomial: binom = binom * (i-k) / k */
                binom = binom * (seq_t)(i - k);
                binom = binom / (seq_t)k;
                /* Update power of 3 */
                pow3 = pow3 * (seq_t)3;
            }
        }

        /* a(i) = 2 * a(i-1) + sum */
        cache[i] = (seq_t)2 * cache[i - 1] + sum;
    }

    len = n + 1;
    return cache[n];
}
