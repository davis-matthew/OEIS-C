/*
 * A366385  Divide n by its largest prime factor, then multiply with the index
 *          of that same prime; a(1) = 0 by convention.
 * https://oeis.org/A366385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n / largest_prime_factor(n)) *
 *            index_of_largest_prime_factor(n)
 * Range:     u8 n <= 383, u16 n <= 8751, u32 n <= 8751, u64 n <= 8751,
 *            u128 n <= 8751, big any n
 * Helpers:   build_primes_upto, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A366385. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/largest_prime_factor.h"
#include "../utils/build_primes_upto.h"

/* Static cache for primes up to the current limit */
static uint64_t *g_primes = NULL;
static size_t   g_prime_count = 0;
static idx_t    g_prime_limit = 0;

/* Ensure we have all primes up to at least `n`. */
static void
ensure_primes(idx_t n)
{
    if (n <= g_prime_limit)
        return;

    /* Free old cache if it exists */
    if (g_primes != NULL)
        free(g_primes);

    build_primes_upto(n, &g_primes, &g_prime_count);
    g_prime_limit = n;
}

/* Binary search for the 1‑based index of prime `p` in `g_primes`. */
static idx_t
prime_index(uint64_t p)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)g_prime_count - 1;

    while (lo <= hi)
    {
        idx_t mid = (lo + hi) >> 1;
        uint64_t val = g_primes[mid];

        if (val == p)
            return (idx_t)(mid + 1);   /* primes[0] is 2, index 1 */

        if (val < p)
            lo = mid + 1;
        else
        {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return 0; /* Should never happen for valid inputs */
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    /* Handle the special 1‑based offset */
    if (n == 1)
        return (seq_t)0;

    /* Ensure primes are available up to n (largest prime factor ≤ n) */
    ensure_primes(n);

    /* Find largest prime factor (p) */
    uint64_t p = largest_prime_factor((uint64_t)n);

    /* Find its 1‑based index */
    idx_t p_idx = prime_index(p);

    /* Compute (n / p) * p_idx safely */
    /* n and p fit in idx_t (uint64_t) */
    uint64_t n_div_p = (uint64_t)n / p;
    uint64_t result  = n_div_p * (uint64_t)p_idx;

    return (seq_t)result;
}
