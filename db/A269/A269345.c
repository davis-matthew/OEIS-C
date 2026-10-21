/*
 * A269345  Smaller of two consecutive odd numbers that are composites.
 * https://oeis.org/A269345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A269345: a(n) = smaller of two consecutive odd composites
   (the n‑th odd number m such that m and m+2 are both composite). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static seq_t *cache      = NULL;   /* cached terms                          */
static size_t cache_len  = 0;       /* number of elements cached             */
static size_t cache_cap  = 0;       /* allocated capacity of the cache       */
static seq_t candidate   = 9;       /* current odd number under consideration */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, return 0 */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* Search for the next odd composite that has an odd composite successor. */
        while (true) {
            if (!is_prime_big(candidate)) {
                seq_t nxt = candidate + 2;
                if (!is_prime_big(nxt)) {
                    break;            /* candidate and nxt are both composite */
                }
            }
            candidate += 2;            /* try the next odd number */
        }

        /* Ensure there is space in the cache. */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* memory failure */
            cache      = tmp;
            cache_cap  = new_cap;
        }

        /* Store the found term. */
        cache[cache_len++] = candidate;

        /* Advance to the next odd number after the stored one. */
        candidate += 2;
    }

    return cache[n - 1];
}
