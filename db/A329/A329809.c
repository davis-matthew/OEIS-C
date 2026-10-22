/*
 * A329809  Numbers k such that floor(k/10)^(k mod 10) contains the digit (k
 *          mod 10).
 * https://oeis.org/A329809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 35, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   big_pow, digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = the n-th integer k such that floor(k/10)^(k mod 10) contains the digit (k mod 10) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache      = NULL;   /* array of found k */
    static size_t cache_len  = 0;      /* number of elements stored */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static idx_t  last_k     = 0;      /* last candidate examined */

    if (n == 0) return 0; /* offset is 1, but should never be called with 0 */

    /* return from cache if possible */
    if ((size_t)n <= cache_len) {
        return cache[n - 1];
    }

    /* extend cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* examine next candidate k */
        last_k = last_k + 1;

        uint64_t d = last_k % 10;
        if (d == 0) continue;          /* skip, digit 0 never appears in k^0 = 1 */

        uint64_t q = last_k / 10;       /* floor(k/10) */

        /* compute power q^d (big integer) */
        seq_t pow_val = big_pow((seq_t)q, (seq_t)d);

        /* check if digit d occurs in different decimal expansion of pow_val */
        unsigned int freq[10];
        digit_frequencies(pow_val, freq);
        if (freq[d] > 0) {
            /* make room if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();      /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* store value */
            cache[cache_len++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
