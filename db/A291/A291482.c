/*
 * A291482  Expansion of e.g.f. arcsin(x)*exp(x).
 * https://oeis.org/A291482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0, a(1) = 1, a(2) = 2, a(3) = 4.
   For n >= 4:
      a(n) = 2*a(n-1) + (n^2-4n+3)*a(n-2)
             - (n-2)*(2n-5)*a(n-3) + (n-3)*(n-2)*a(n-4).
 */
#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;          /* number of elements stored */

    /* Initialize cache with first four elements if necessary */
    if (cached_size == 0) {
        cached_size = 4;
        cache = (seq_t *)calloc(cached_size, sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 4;
    }

    /* Grow cache to hold up to index n (inclusive) */
    if (n + 1 > cached_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Compute new terms using the recurrence */
        for (idx_t k = cached_size; k < new_size; ++k) {
            /* k is the target index, n = k-4 in the original recurrence */
            seq_t k_seq = (seq_t)k;

            /* 2*a(k-1) */
            seq_t term1 = cache[k - 1] + cache[k - 1];

            /* (k^2 - 4k + 3) * a(k-2) */
            seq_t coeff1 = k_seq * k_seq - (seq_t)4 * k_seq + (seq_t)3;
            seq_t term2 = coeff1 * cache[k - 2];

            /* -(k-2)*(2k-5) * a(k-3) */
            seq_t coeff2 = (k_seq - (seq_t)2) * ((seq_t)2 * k_seq - (seq_t)5);
            seq_t term3 = coeff2 * cache[k - 3];

            /* +(k-3)*(k-2) * a(k-4) */
            seq_t coeff3 = (k_seq - (seq_t)3) * (k_seq - (seq_t)2);
            seq_t term4 = coeff3 * cache[k - 4];

            cache[k] = term1 + term2 - term3 + term4;
        }
        cached_size = new_size;
    }

    return cache[n];
}
