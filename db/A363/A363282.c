/*
 * A363282  Squares (A000290) and centered squares (A001844), in increasing
 *          order (i.e., sorted and without duplicates).
 * https://oeis.org/A363282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 432, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A363282: merge of squares (n^2) and centered squares (2n^2+2n+1),
   sorted increasingly and without duplicates. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Treat index 0 as undefined but harmless. */
    if (n == 0) return (seq_t)0;

    /* Static cache for computed values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Indices for the two component sequences. */
    static idx_t sq_idx = 1;   /* n for n²  (n ≥ 1) */
    static idx_t cs_idx = 0;   /* n for 2n²+2n+1 (n ≥ 0) */

    /* Generate values until we have at least n terms. */
    while (cache_len < n) {
        /* Calculate next candidate from the square sequence. */
        seq_t valSq = ((seq_t)sq_idx) * ((seq_t)sq_idx);

        /* Calculate next candidate from the centered‑square sequence. */
        seq_t cs_sq = ((seq_t)cs_idx) * ((seq_t)cs_idx);
        seq_t valCs = cs_sq * ((seq_t)2) + ((seq_t)cs_idx) * ((seq_t)2) + ((seq_t)1);

        /* Choose the smaller candidate and remember where it came from. */
        seq_t next_val;
        int fromSq;  /* 1 = square only, 2 = centered only, 3 = both equal */

        if (valSq <= valCs) {
            next_val = valSq;
            fromSq   = (valSq == valCs) ? 3 : 1;
        } else {
            next_val = valCs;
            fromSq   = 2;
        }

        /* Append the value if it is strictly greater than the last one added. */
        if (cache_len == 0 || next_val > cache[cache_len - 1]) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_val;
        }

        /* Advance the indices that produced the chosen value. */
        switch (fromSq) {
            case 1: sq_idx++; break;
            case 2: cs_idx++; break;
            case 3: sq_idx++; cs_idx++; break;
        }
    }

    return cache[(size_t)(n - 1)];
}
