/*
 * A371654  Numbers that are not multiples of 10 and that yield a prime if
 *          their digits are arranged in ascending order.
 * https://oeis.org/A371654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 7715, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by scanning each integer, skipping multiples of 10, and testing whether
 * the integer formed by its digits in ascending order is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"
#include "../utils/is_prime_big.h"

/* Return the n-th term of sequence A371654 (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache to allow repeated calls in any order. */
    static seq_t *cache = NULL;           /* array of found terms          */
    static size_t len   = 0;              /* number of terms stored        */
    static size_t cap   = 0;              /* allocated capacity            */
    static seq_t last_checked = 1;        /* largest integer already tested */

    /* Ensure the cache contains at least n terms. */
    if (len < (size_t)n) {
        /* Reallocate cache if necessary. */
        if (cap == 0) {
            cap = 8;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
        } else if (len == cap) {
            cap <<= 1;
            cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        }

        /* Generate terms until we have n of them. */
        seq_t cand = last_checked + 1;
        while (len < (size_t)n) {
            /* Skip multiples of 10. */
            if ((cand % 10) != 0) {
                /* Build the number formed by the digits of cand in ascending order. */
                unsigned char cnt[10];
                digit_counts(cand, cnt);
                seq_t asc = 0;
                for (int d = 0; d < 10; ++d) {
                    for (int i = 0; i < cnt[d]; ++i) {
                        asc = asc * 10 + d;
                    }
                }
                if (is_prime_big(asc)) {
                    /* Term found; store it. */
                    if (len == cap) {
                        cap <<= 1;
                        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                    }
                    cache[len++] = cand;
                }
            }
            cand++;
            last_checked = cand - 1;
        }
    }
    return cache[n - 1];
}
