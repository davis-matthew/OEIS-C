/*
 * A291669  Numbers k such that 1 is the smallest decimal digit of k^4.
 * https://oeis.org/A291669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   big_pow, digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search:  a(n) is the n‑th integer k such that
   the smallest decimal digit of k^4 equals 1.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/big_pow.h"
#include "../utils/digit_frequencies.h"

/* Return the n-th element (offset 1) of A291669 */
seq_t a(idx_t n)
{
    /* Internal cache for arbitrary‑order calls */
    static seq_t *cache      = NULL;   /* stored terms */
    static size_t cache_len  = 0;      /* #terms currently stored */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static idx_t  next_k     = 1;      /* next k to test */

    /* Extend the cache until it contains at least n terms */
    while (cache_len < (size_t)n)
    {
        seq_t k   = (seq_t)next_k;         /* candidate integer */
        seq_t k4  = big_pow(k, (seq_t)4); /* k^4 */

        unsigned int freq[10] = {0};
        digit_frequencies(k4, freq);

        /* Accept if 0 does not occur and 1 does occur */
        if (freq[0] == 0 && freq[1] > 0)
        {
            /* Grow cache if needed */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = k;
        }

        ++next_k;
    }

    return cache[n - 1];
}
