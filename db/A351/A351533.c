/*
 * A351533  Odd numbers k that are multiples of 3 and for which sigma(k) is
 *          congruent to 2 modulo 4.
 * https://oeis.org/A351533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A351533: odd multiples of 3 with σ(k) ≡ 2 (mod 4), listed in increasing order.
 * The function searches sequentially and caches results so that subsequent
 * calls are fast.  It uses the provided sigma_of(idx_t) utility.
 */

#include <stdlib.h>
#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n) */

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* cached values of the sequence */
    static seq_t *cache    = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* next odd multiple of 3 to examine */
    static idx_t next_cand = 3;

    /* generate terms until the cache contains at least n values */
    while (cache_len < n) {
        idx_t cand   = next_cand;
        next_cand   += 6;           /* advance to the next odd multiple of 3 */

        /* sigma_of expects an idx_t argument */
        seq_t sigma = sigma_of(cand);
        if ((sigma % (seq_t)4) == (seq_t)2) {
            /* grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
