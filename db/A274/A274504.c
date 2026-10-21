/*
 * A274504  Primes with Kronecker symbol (p|14) != 1.
 * https://oeis.org/A274504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, keep_prime, sig2, sig7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p with Kronecker symbol (p|14) != 1 */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/keep_prime.h"
#include "../utils/sig2.h"
#include "../utils/sig7.h"

/* Cached list of terms */
static seq_t *cache    = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t last_checked = 1;   /* number just checked */

/* a(n) returns the n-th element (1‑indexed).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n < 1 */

    while (cache_len < n) {
        ++last_checked;               /* next integer to examine */
        if (!is_prime(last_checked))  /* skip composites */
            continue;

        if (keep_prime(last_checked)) {   /* add to cache */
            if (cache_len + 1 > cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
