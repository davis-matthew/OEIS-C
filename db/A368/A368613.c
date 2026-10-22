/*
 * A368613  a(n) = Sum_{k=2..n} pi(k-1) * floor(n/k).
 * https://oeis.org/A368613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2..n} pi(k-1) * floor(n/k) (offset 1)
 * Range:     u8 n <= 29, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple sieve for pi(x) and caches the results
 * across calls so that a(n) can be evaluated in O(n) time. */

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Cached prime counting array                                         */
/* --------------------------------------------------------------------- */
static idx_t      max_pi_cached = 0;      /* furthest index for which pi_cache is valid   */
static uint32_t * pi_cache      = NULL;    /* pi_cache[i] = pi(i) for 0 <= i <= max_pi_cached */

/* --------------------------------------------------------------------- */
/* Ensure that pi_cache covers at least n.                              */
/* --------------------------------------------------------------------- */
static void
ensure_pi_cache(idx_t n)
{
    if (n <= max_pi_cached)               /* already available */
        return;

    /* Allocate temporary boolean array for the sieve */
    bool *is_prime = (bool *)calloc((size_t)(n + 1), sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);    /* out of memory */

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i <= n; ++i) is_prime[i] = true;
    for (idx_t p = 2; p <= n / p; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t m = p * p; m <= n; m += p)
            is_prime[m] = false;
    }

    /* Allocate or grow pi_cache to hold values up to n */
    uint32_t *new_cache = (uint32_t *)realloc(pi_cache, (size_t)(n + 1) * sizeof(uint32_t));
    if (!new_cache) {
        free(is_prime);
        exit(EXIT_FAILURE);
    }
    pi_cache = new_cache;

    /* Build prefix counts pi(i) */
    pi_cache[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        pi_cache[i] = pi_cache[i - 1] + (is_prime[i] ? 1 : 0);

    max_pi_cached = n;
    free(is_prime);
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n < 2)                 /* a(1) = 0; a(0) not defined but returns 0 */
        return (seq_t)0;

    ensure_pi_cache(n);        /* make sure pi(k-1) exists for all k <= n */

    seq_t total = 0;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t pk       = (seq_t)pi_cache[k - 1];   /* pi(k-1) */
        seq_t floor_nk = (seq_t)(n / k);          /* floor(n/k) */
        total += pk * floor_nk;
    }
    return total;
}
