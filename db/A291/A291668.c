/*
 * A291668  Numbers k such that 0 is the smallest decimal digit of k^4.
 * https://oeis.org/A291668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   big_pow, digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A291668: numbers k such that 0 is the smallest decimal
 * digit of k^4, i.e. k^4 contains at least one digit '0'.
 * The implementation builds the sequence on demand and memoises it.
 * Complexity: each new element requires at most one big‑pow
 * computation; total cost is O(n) for n results generated cumulatively.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"          /* big_pow(seq_t, seq_t) */
#include "../utils/digit_counts.h"     /* digit_counts(seq_t, unsigned char[10]) */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0, cache_cap = 0;
    static idx_t next_k = 1;   /* next integer to test */

    /* generate terms until we have at least n of them */
    while (cache_len < n)
    {
        seq_t power = big_pow((seq_t)next_k, (seq_t)4); /* k^4 */
        unsigned char cnt[10];
        digit_counts(power, cnt);                     /* counts of each decimal digit */

        if (cnt[0] > 0)          /* contains a '0' digit */
        {
            if (cache_len == cache_cap)            /* grow cache if needed */
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();                  /* abort on allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];          /* a(1) is cache[0] */
}
