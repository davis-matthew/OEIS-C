/*
 * A333636  a(n) is the greatest least part of a partition of n into prime
 *          parts which does not divide n, or 0 if no such prime exists.
 * https://oeis.org/A333636
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest prime p ≤ n, p∤n, such that n can be partitioned
 *            into primes all ≥ p with least part p (equivalently, n-p is
 *            representable by primes ≥ p). 0 if no such prime exists.
 *            Sequence A333636
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Static state for memoisation */
static idx_t maxN = 0;
static seq_t *best = NULL;          /* best[n] = a(n) for 0 ≤ n ≤ maxN */
static bool *reachable = NULL;      /* reachable[s] = true if s is sum of primes ≥ current p */
static uint64_t *primes = NULL;     /* list of primes up to maxN */
static size_t prime_count = 0;

/* Free existing cached data */
static void free_cache(void)
{
    if (best) free(best);
    if (reachable) free(reachable);
    if (primes) free(primes);
    best = NULL;
    reachable = NULL;
    primes = NULL;
    prime_count = 0;
}

/* Recompute data up to n_max (inclusive) */
static void recompute(idx_t n_max)
{
    free_cache();

    maxN = n_max;

    /* Generate primes up to n_max */
    build_primes_upto(n_max, &primes, &prime_count);

    /* Allocate arrays */
    reachable = (bool*)calloc((size_t)(n_max + 1), sizeof(bool));
    best      = (seq_t*)calloc((size_t)(n_max + 1), sizeof(seq_t));
    if (!reachable || !best) {
        /* Allocation failed – terminate */
        abort();
    }

    reachable[0] = true;  /* empty sum is reachable */

    /* Process primes from largest to smallest */
    for (size_t pi = prime_count; pi-- > 0; ) {
        idx_t p = (idx_t)primes[pi];

        /* Update reachable for sums using coin p (unbounded) */
        for (idx_t s = p; s <= n_max; ++s) {
            if (!reachable[s] && reachable[s - p]) {
                reachable[s] = true;
            }
        }

        /* Assign best[n] = p for all n where p is a valid least part */
        if (p <= n_max) {
            for (idx_t n = p; n <= n_max; ++n) {
                if (best[n] == (seq_t)0 &&
                    reachable[n - p] &&
                    (n % p != 0)) {
                    best[n] = (seq_t)p;
                }
            }
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    if (n > maxN) recompute(n);
    return best[n];
}
