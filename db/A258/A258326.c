/*
 * A258326  a(1) = 3; for n > 1, a(n) = a(n-1) + prime(n+2) - 2*prime(n+1) +
 *          2*prime(n) - prime(n-1).
 * https://oeis.org/A258326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3; for n>1,
 *            a(n)=a(n-1)+prime(n+2)-2*prime(n+1)+2*prime(n)-prime(n-1)
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;      /* highest index computed */
    static size_t cache_len = 0;    /* allocated array length (0 unused) */

    if (n > cached_n) {
        /* enlarge cache if needed */
        size_t new_len = (size_t)(n + 1);   /* indices 0 .. n */
        if (cache_len < new_len) {
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_len = new_len;
        }

        /* initialise first term if not yet done */
        if (cached_n == 0) {
            cache[1] = (seq_t)3;
            cached_n = 1;
        }

        /* compute subsequent terms up to n */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t p_prev  = (seq_t)get_prime(i - 1);
            seq_t p_curr  = (seq_t)get_prime(i);
            seq_t p_next  = (seq_t)get_prime(i + 1);
            seq_t p_next2 = (seq_t)get_prime(i + 2);

            seq_t val = cache[i - 1];
            val += p_next2;
            val -= ((seq_t)2 * p_next);
            val += ((seq_t)2 * p_curr);
            val -= p_prev;

            cache[i] = val;
        }
        cached_n = n;
    }
    return cache[n];
}
