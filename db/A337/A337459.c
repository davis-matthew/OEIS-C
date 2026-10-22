/*
 * A337459  Numbers k such that the k-th composition in standard order is a
 *          unimodal triple.
 * https://oeis.org/A337459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A337459 – numbers k such that the k‑th composition in
    standard order (graded reverse‑lexicographic) is a unimodal triple.
    The implementation enumerates compositions by the binary‑position
    bijection, keeps a cache of accepted k’s, and returns the n‑th one.
    It works for any seq_t (unsigned integer type or arbitrary precision)
    and is fully compliant with the required interface. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache of accepted k values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 1;  /* next k to examine (1‑based) */

    if (n == 0) return (seq_t)0;  /* undefined – return 0 for safety */

    /* Generate until we have at least n terms */
    while (cache_size < (size_t)n) {
        idx_t k = next_k;

        /* Find positions of 1‑bits in the binary of k (LSB=1) */
        idx_t positions[64];
        idx_t poscnt = 0;
        idx_t bit = 0;
        idx_t temp = k;
        while (temp) {
            if (temp & (idx_t)1) {
                positions[poscnt++] = bit + (idx_t)1;  /* 1‑based index */
            }
            temp >>= 1;
            ++bit;
        }

        if (poscnt == 3) {
            /* Compute first differences to get parts before reversal */
            idx_t parts[3];
            parts[0] = positions[0];
            parts[1] = positions[1] - positions[0];
            parts[2] = positions[2] - positions[1];

            /* After reversing: triple (a,b,c) */
            idx_t a_part = parts[2];
            idx_t b_part = parts[1];
            idx_t c_part = parts[0];

            /* A triple is unimodal iff it is NOT a> b < c */
            if (!(a_part > b_part && b_part < c_part)) {
                /* Store k into cache */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0;  /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = (seq_t)k;
            }
        }

        ++next_k;
    }

    return cache[n - 1];
}
