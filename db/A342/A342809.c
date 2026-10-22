/*
 * A342809  Numbers k such that k-1 and round(k/5) are both prime.
 * https://oeis.org/A342809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   k such that k-1 and round(k/5) are primes. Computed by
 *            brute‑force search with memoisation
 * Range:     u8 n <= 8, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache holding all already found terms */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* next candidate value of k */
    static seq_t next_k = 1;

    /* Initialise the cache on the first call */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* seek next k such that k-1 and round(k/5) are prime */
        while (true) {
            /* Test k-1 */
            if (is_prime_big(next_k - 1)) {
                /* rounded(k/5) = floor((k+2)/5) */
                seq_t r = (next_k + 2) / (seq_t)5;
                if (is_prime_big(r)) {
                    /* cache this term */
                    if (cache_len + 1 > cache_cap) {
                        cache_cap *= 2;
                        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                        if (!tmp) exit(1);
                        cache = tmp;
                    }
                    cache[cache_len++] = next_k;
                    break;          /* go on to the next term */
                }
            }
            /* try the next integer */
            ++next_k;
        }
        /* advance to the following integer after the one just processed */
        ++next_k;
    }

    return cache[n - 1];
}
