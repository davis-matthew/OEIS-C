/*
 * A355591  a(n) = (product of the first n odd primes) - (sum of the first n
 *          odd primes).
 * https://oeis.org/A355591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (product of the first n odd primes) - (sum of the first n
 *            odd primes)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/sieve_first_n_primes.h"

/* Cached array of the first primes including 2 (prime[0] = 2, prime[1] = 3, etc.) */
static uint32_t *g_primes = NULL;
static size_t    g_primes_count = 0;

/* Ensure that g_primes contains at least n+1 primes (including 2 and the first n odd primes). */
static void ensure_primes(idx_t n)
{
    /* We need primes[0] = 2 plus primes[1..n] = first n odd primes. */
    if (g_primes_count < (size_t)n + 1) {
        free(g_primes);          /* free previous buffer if any */
        g_primes = NULL;
        g_primes_count = 0;

        uint32_t *tmp = NULL;
        size_t    cnt = 0;
        /* The utility produces the first (n+1) primes, zero‑based indexing. */
        sieve_first_n_primes((idx_t)n + 1, &tmp, &cnt);

        g_primes = tmp;
        g_primes_count = cnt;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Empty product and sum; by convention a(0) = 1. */
        return (seq_t)1;
    }

    ensure_primes(n);

    seq_t prod = (seq_t)1;
    seq_t sum  = (seq_t)0;

    /* Iterate over indices 1..n: prime[i] are the first n odd primes. */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t p = (seq_t)g_primes[(size_t)i];
        prod *= p;
        sum  += p;
    }

    return prod - sum;
}
