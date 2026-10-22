/*
 * A301277  Nearest integer to mean of first n primes.
 * https://oeis.org/A301277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(A007504(n) / n), where A007504(n) is the sum of the
 *            first n primes
 * Range:     u8 n <= 13, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Warning: all arithmetic is integer; round-to-nearest-half-up is achieved by (sum + n/2)/n. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *g_primes = NULL;   /* array of the first g_nmax primes */
static idx_t      g_nmax  = 0;      /* number of primes currently stored */

seq_t a(idx_t n)
{
    /* Guard against an impossible call with n == 0, although the OEIS offset is 1. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure we have at least n primes available. */
    if (n > g_nmax) {
        size_t  cnt;
        uint32_t *pr = NULL;

        /* Generate the first n primes. */
        sieve_first_n_primes(n, &pr, &cnt);

        /* Free previously allocated primes array, if any. */
        free(g_primes);

        /* Store the new primes array. */
        g_primes = pr;
        g_nmax   = (idx_t)cnt;   /* cnt == n by contract of sieve_first_n_primes */
    }

    /* Compute the sum of the first n primes. */
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        sum += (seq_t)g_primes[i];
    }

    /* Return the nearest integer to sum / n (ties rounded up). */
    seq_t result = (sum + (seq_t)(n / 2)) / (seq_t)n;
    return result;
}
