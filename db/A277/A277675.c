/*
 * A277675  Numbers k such that d(k+2) = d(k+1), where d(m) is the number of
 *          digits in the base-m representation of m!.
 * https://oeis.org/A277675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits_factorial_in_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that d(k+2) = d(k+1), where
   d(m) = floor( ln(m!)/ln(m) ) + 1 (number of digits of m! in base m). */

#include <stdlib.h>
#include <math.h>
#include "../utils/digits_factorial_in_base.h"

/* Return the nth term of the sequence.
   The implementation lazily generates terms and remembers
   all terms seen so far in a dynamically allocated array. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next_k = 1;  /* next k to test */

    if (n < 1) return 0;  /* undefined for n < 1 */

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        idx_t d1 = digits_factorial_in_base(next_k + 1);
        idx_t d2 = digits_factorial_in_base(next_k + 2);

        if (d1 == d2) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                void *tmp = realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failure */
                cache = (seq_t *)tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
