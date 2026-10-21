/*
 * A261447  The number of distinct prime factors of the number of proper
 *          divisors of n.
 * https://oeis.org/A261447
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ω(τ(n)-1) where τ(n) is the number of divisors of n
 *            (including n) and ω(m) is the number of distinct prime factors
 *            of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_spf.h"          /* void build_spf(idx_t limit, idx_t **out); */
#include "../utils/count_distinct_primes.h" /* int count_distinct_primes(uint64_t n); */

static idx_t current_limit = 0;   /* highest n for which the SPF table is valid */
static idx_t *spf = NULL;         /* smallest prime factor table */

/* Ensure that spf[0..n] is built. Locks in the current limit. */
static void
ensure_spf(idx_t n)
{
    if (n <= current_limit) return;

    /* free previous table, if any */
    if (spf) free(spf);
    idx_t *new_spf = NULL;
    build_spf(n, &new_spf);   /* utility allocates the array */
    spf = new_spf;
    current_limit = n;
}

/* Compute τ(n) using the SPF table; return as uint64_t (fits for n ≤ 2^64). */
static uint64_t
tau_uint64(idx_t n)
{
    uint64_t tau = 1;
    idx_t x = n;
    while (x > 1) {
        idx_t p = spf[x];
        idx_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        tau *= (exp + 1);
    }
    return tau;
}

/* External function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* the sequence is defined for n ≥ 2 */
    ensure_spf(n);
    uint64_t tau = tau_uint64(n);
    uint64_t proper = tau - 1;            /* number of proper divisors */
    int omega = count_distinct_primes(proper);
    return (seq_t)omega;
}
