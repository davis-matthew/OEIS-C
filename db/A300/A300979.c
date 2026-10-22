/*
 * A300979  Number of partitions of n into parts having the same number of
 *          distinct prime divisors as n.
 * https://oeis.org/A300979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 70, u32 n <= 222, u64 n <= 820,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute a(n) = number of partitions of n into parts whose number of
   distinct prime divisors equals ω(n) (number of distinct prime
   divisors of n).  The computation uses a dynamic‑programming
   sieve over all integers k ≤ n with ω(k)=ω(n).  */

/* Static cache for number of distinct prime divisors (ω) of numbers
   up to the largest n seen so far.  Overwrite the cache whenever a
   larger n is requested.  */
static seq_t *omega_cache = NULL;
static size_t  omega_cap   = 0;

/* Ensure omega_cache contains ω(k) for all 0 ≤ k ≤ n.  */
static void
ensure_omega_cache(idx_t n)
{
    if (n < omega_cap)   /* already computed */
        return;

    /* Allocate a fresh array and initialise to 0. */
    size_t cap = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)calloc(cap, sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);

    /* Sieve to count distinct prime factors. */
    for (idx_t i = 2; i <= n; ++i) {
        if (new_cache[i] == 0) {          /* i is prime */
            for (idx_t j = i; j <= n; j += i)
                new_cache[j] += 1;
        }
    }

    free(omega_cache);
    omega_cache = new_cache;
    omega_cap   = cap;
}

/* Public function requested by the build system. */
seq_t
a(idx_t n)
{
    /* a(0) = 1 by definition (empty partition). */
    if (n == 0)
        return (seq_t)1;

    /* Compute ω(k) for all k ≤ n. */
    ensure_omega_cache(n);

    idx_t omega_n = (idx_t)omega_cache[n];

    /* Dynamic programming array for partition counts. */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        exit(EXIT_FAILURE);

    dp[0] = (seq_t)1;   /* one way to form 0 */

    /* For each integer k with ω(k)=ω(n), update dp. */
    for (idx_t k = 1; k <= n; ++k) {
        if ((idx_t)omega_cache[k] == omega_n) {
            for (idx_t j = k; j <= n; ++j) {
                dp[(size_t)j] += dp[(size_t)(j - k)];
            }
        }
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    return result;
}
