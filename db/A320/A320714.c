/*
 * A320714  Indices of primes followed by a gap (distance to next larger
 *          prime) of 32.
 * https://oeis.org/A320714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index i of prime sequence such that prime(i+1) =
 *            prime(i)+32 (computed by searching for prime gaps of size 32
 *            with a sieve)
 * Range:     u8 none, u16 n <= 432, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static seq_t *gap_indices = NULL;
static size_t gap_count = 0;
static size_t gap_cap = 0;
static idx_t sieve_limit = 0;
static bool cache_built = false;

static void ensure_gap_indices(idx_t needed) {
    if (cache_built && gap_count >= needed)
        return;

    idx_t limit = (sieve_limit == 0) ? 100000 : sieve_limit * 2;

    for (;;) {
        uint64_t *primes = NULL;
        size_t prime_cnt = 0;
        build_primes_upto(limit, &primes, &prime_cnt);

        size_t estimated = prime_cnt; /* maximum possible gaps */
        if (!gap_indices || gap_cap < estimated) {
            free(gap_indices);
            gap_indices = (seq_t*)malloc(estimated * sizeof(seq_t));
            gap_cap = estimated;
        }

        size_t g = 0;
        for (size_t i = 0; i + 1 < prime_cnt; ++i) {
            if (primes[i + 1] - primes[i] == 32) {
                gap_indices[g++] = (seq_t)(i + 1); /* 1‑based index in prime list */
            }
        }

        free(primes);
        gap_count = g;
        sieve_limit = limit;

        if (gap_count >= needed) {
            cache_built = true;
            return;
        }
        limit *= 2;
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    ensure_gap_indices(n);
    return gap_indices[n - 1];
}
