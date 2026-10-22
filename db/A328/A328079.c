/*
 * A328079  First differences of A328078.
 * https://oeis.org/A328079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328079: first differences of A328078.
   Initial terms (n = 0..11):
      1, 1, 2, 4, 6, 12, 21, 43, 78, 156, 293, 583
   For n ≥ 12 the recurrence holds:
      a(n) = a(n-1) + 3*a(n-2) - 3*a(n-3) + 4*a(n-4)
             - 12*a(n-6) + 8*a(n-7) - 4*a(n-8) - 4*a(n-9)
             + 8*a(n-10) - 8*a(n-11)           */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* seed values for n = 0..11 */
    static const seq_t seed[12] = {
        (seq_t)1,  (seq_t)1,  (seq_t)2,  (seq_t)4,
        (seq_t)6,  (seq_t)12, (seq_t)21, (seq_t)43,
        (seq_t)78, (seq_t)156,(seq_t)293,(seq_t)583
    };

    /* initialise cache on first call */
    if (cache == NULL) {
        size_t init_cap = 64;
        if (n + 1 > init_cap) init_cap = (size_t)n + 1;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* unlikely, but safe */
        for (size_t i = 0; i < 12; ++i) cache[i] = seed[i];
        cache_len = 12;
        cache_cap = init_cap;
    }

    /* if the requested index is already cached, return */
    if (n < cache_len) return cache[(size_t)n];

    /* extend storage if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* recurrence coefficients for a(n) in terms of the 11 previous terms */
    const int coeff[11] = {  1,  3, -3,  4,  0, -12,  8, -4, -4,  8, -8 };

    /* compute the sequence forward up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t pos = 0;   /* sum of positive‑coeff terms */
        seq_t neg = 0;   /* sum of negative‑coeff terms (absolute value) */

        for (int j = 0; j < 11; ++j) {
            idx_t idx_prev = (idx_t)(i - 1 - j);
            int c = coeff[j];
            if (c > 0) {
                pos += (seq_t)c * cache[idx_prev];
            } else if (c < 0) {
                neg += (seq_t)(-c) * cache[idx_prev];
            }
        }
        /* parities guarantee pos ≥ neg, so subtraction is safe */
        cache[i] = pos - neg;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
