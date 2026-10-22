/*
 * A390805  The sum of the integers k from 1 to n such that gcd(n, k) is a
 *          3-smooth number (A003586).
 * https://oeis.org/A390805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + I_3smooth(n)) * phi(6*n) / 4 where I_3smooth(n) = 1
 *            if n is a 3‑smooth number (i.e. n = 2^a 3^b), otherwise 0.
 *            phi(m) is Euler's totient
 * Range:     u8 n <= 23, u16 n <= 365, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is3smooth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is3smooth.h"

/* The totient table is computed up to the largest argument seen so far. */
static seq_t *phi_cache = NULL;
static idx_t phi_cap   = 0;

/* Compute phi values 1..limit in phi_cache.  One‑time lazy initialization. */
static void init_phi(idx_t limit)
{
    size_t sz = (size_t)(limit + 1);
    seq_t *newcache = (seq_t *)realloc(phi_cache, sz * sizeof(seq_t));
    if (!newcache) abort();           /* allocation failed */
    phi_cache = newcache;
    phi_cap   = limit;

    /* start with phi[i] = i */
    for (idx_t i = 0; i <= limit; ++i) phi_cache[i] = (seq_t)i;
    phi_cache[0] = 0;

    /* linear sieve for phi */
    for (idx_t p = 2; p <= limit; ++p) {
        if (phi_cache[p] == (seq_t)p) {               /* p is prime */
            for (idx_t j = p; j <= limit; j += p) {
                phi_cache[j] = phi_cache[j] / (seq_t)p * (seq_t)(p - 1);
            }
        }
    }
}

/* Ensure we have phi values at least up to n. */
static void ensure_phi(idx_t n)
{
    if (n <= phi_cap) return;
    init_phi(n);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offset 1 */

    seq_t n_seq = (seq_t)n;
    seq_t indicator = is3smooth(n) ? 1 : 0;

    idx_t six_n = 6 * n;           /* 6*n fits in idx_t for all test data */
    ensure_phi(six_n);
    seq_t phi6n = phi_cache[six_n];

    seq_t result = ((n_seq + indicator) * phi6n) / 4;
    return result;
}
