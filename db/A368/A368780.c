/*
 * A368780  Cubefree numbers divisible by the square of their largest prime
 *          factor.
 * https://oeis.org/A368780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 781, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A368780: cubefree numbers divisible by the square of
 * their largest prime factor.
 * A number n satisfies:
 *   - n is cube‑free (no prime divisor has exponent ≥ 3);
 *   - let p be the largest prime dividing n,
 *     then p² divides n.
 * */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    /* No meaningful result for n == 0; sequence starts at n == 1 */
    if (n == 0) return (seq_t)0;

    /* Cache of already found terms (zero‑based index). */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of cached terms      */
    static size_t cache_cap = 0;   /* current allocation capacity */
    static idx_t last_checked = 3; /* last integer examined      */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Next candidate integer to test. */
        idx_t cand = last_checked + 1;
        last_checked = cand;

        /* Factorise cand.  The utility returns arrays of primes and
         * exponent counts. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t pcnt = 0;
        factor_u64((uint64_t)cand, primes, exps, &pcnt);

        /* Check cube‑free and that the largest prime has exponent 2. */
        bool cube_free = true;
        uint64_t max_p = 0, max_exp = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            if (exps[i] > 2) {
                cube_free = false;
                break;
            }
            if (primes[i] > max_p) {
                max_p = primes[i];
                max_exp = exps[i];
            }
        }
        if (!cube_free || max_exp < 2)
            continue; /* candidate does not belong to the sequence */

        /* Ensure we have enough space to store the new term. */
        if (cache_len >= cache_cap) {
            /* Grow the cache by at least one element. */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
        }

        /* Store the valid term. */
        cache[cache_len++] = (seq_t)cand;
    }

    /* Terms are stored with zero‑based index; return the (n‑1)th. */
    return cache[n - 1];
}
