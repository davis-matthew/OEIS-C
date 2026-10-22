/*
 * A305558  If (p1,p2) is the n-th twin prime pair and p the prime before p1
 *          and q the prime after p2 then a(n) = p + q - (p1 + p2).
 * https://oeis.org/A305558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = prime_before(p1) + prime_after(p2) - (p1 + p2) where
 *            (p1,p2) is the n‑th twin prime pair
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions supplied by the build (included from utilities). */
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Internal cache of pre‑computed a(n) values. */
static seq_t   *a_cache = NULL;
static size_t  a_cache_size = 0;
static bool    initialized  = false;

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Lazy initialization: compute all terms up to a safe limit. */
    if (!initialized) {
        /* 10 000 000 is enough to cover the first 10 000 twin primes. */
        const idx_t LIMIT = 10000000ULL;

        uint64_t *primes = NULL;
        size_t   primes_cnt = 0;
        build_primes_upto(LIMIT, &primes, &primes_cnt);

        /* Count twin pairs to allocate the cache. */
        size_t twin_pairs = 0;
        if (primes_cnt >= 3) {
            for (size_t i = 1; i + 1 < primes_cnt; ++i) {
                if (primes[i + 1] == primes[i] + 2)
                    ++twin_pairs;
            }
        }

        a_cache = (seq_t *)malloc(twin_pairs * sizeof(seq_t));
        a_cache_size = twin_pairs;
        size_t pos = 0;

        /* Compute a(n) for each twin pair. */
        if (primes_cnt >= 3) {
            for (size_t i = 1; i + 1 < primes_cnt; ++i) {
                if (primes[i + 1] != primes[i] + 2)
                    continue;

                uint64_t p1 = primes[i];
                uint64_t p2 = primes[i + 1];
                uint64_t p  = primes[i - 1];
                uint64_t q;

                /* Next prime after p2: use array element if present,
                   otherwise increment until a prime is found. */
                if (i + 2 < primes_cnt)
                    q = primes[i + 2];
                else {
                    uint64_t candidate = p2 + 1;
                    while (!is_prime(candidate))
                        ++candidate;
                    q = candidate;
                }

                /* Compute the value in a wider type to avoid overflow
                   before casting it to seq_t. */
                long long v = (long long)p + (long long)q
                              - (long long)p1 - (long long)p2;
                a_cache[pos++] = (seq_t)v;
            }
        }

        free(primes);
        initialized = true;
    }

    /* Bounds check (behaviour for out of range is irrelevant). */
    if (n < 1 || (size_t)n > a_cache_size)
        return (seq_t)0;

    return a_cache[(size_t)n - 1];
}
