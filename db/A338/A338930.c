/*
 * A338930  Numbers k such that Sum_{d | k} (d^2 mod k) is prime.
 * https://oeis.org/A338930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1137, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A338930: numbers k such that Sum_{d|k}(d^2 mod k) is prime.
 * Computation is performed by incremental search and memoisation of the
 * found terms. No closed form is known.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/isPrimeGeneric.h"

/* Recursively generate all divisors of k from its prime factorisation
 * and accumulate the sum of (d^2 mod k).  The sum is kept in 128‑bit
 * integer to avoid overflow.  All arithmetic is performed on native
 * integer types; seq_t is only used when the sum is finally cast
 * for the primality test. */
static void gen_divisors(const uint64_t *primes, const uint64_t *exps,
                         size_t cnt, size_t idx, uint64_t cur, uint64_t k,
                         unsigned __int128 *sum)
{
    if (idx == cnt) {
        /* cur is one divisor of k */
        unsigned __int128 d2 = (unsigned __int128)cur * cur;
        *sum += (d2 % k);
        return;
    }

    uint64_t p = primes[idx];
    uint64_t c = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divisors(primes, exps, cnt, idx + 1, c, k, sum);
        c *= p;
    }
}

/* Main function returning the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;        /* array of previously found k */
    static size_t cache_len = 0;       /* number of cached terms */
    static size_t cache_cap = 0;       /* allocated capacity */
    static uint64_t last_checked = 0;  /* last integer examined */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        ++last_checked; /* candidate k */

        /* Factorisation of k */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(last_checked, primes, exps, &cnt);

        /* Sum over all divisors */
        unsigned __int128 sum128 = 0;
        gen_divisors(primes, exps, cnt, 0, 1, last_checked, &sum128);

        /* Test primality of the sum */
        seq_t sum = (seq_t)sum128;
        if (isPrimeGeneric(sum)) {
            /* Store this k in the cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache,
                                               new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Out of memory – terminate program */
                    exit(EXIT_FAILURE);
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[(size_t)n - 1];
}
