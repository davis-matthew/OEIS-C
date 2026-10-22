/*
 * A332510  a(n) = Sum_{k=1..n} lambda(floor(n/k)), where lambda = A008836.
 * https://oeis.org/A332510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} lambda(floor(n/k)) where lambda is the
 *            Liouville function
 * Range:     u8 n <= 938, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation precomputes λ(i) for i ≤ n using the smallest prime factor sieve. */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for Liouville function values */
static signed char *lambda_cache = NULL;   /* λ(0) unused, λ(1)=1, λ(i)=±1 */
static idx_t lambda_cache_size = 0;       /* number of entries allocated (>=1) */

/* Ensure λ(i) is known for all i ≤ n */
static void ensure_lambda(idx_t n)
{
    if (lambda_cache_size > n)            /* already sufficient (size > n implies size ≥ n+1) */
        return;

    /* Allocate new cache of size n+1 (indices 0..n) */
    idx_t new_size = n + 1;
    signed char *new_cache = (signed char *)calloc((size_t)new_size, sizeof(signed char));
    if (!new_cache)
        exit(1);    /* allocation failed */

    /* Allocate array for smallest prime factor (spf) */
    idx_t *spf = (idx_t *)calloc((size_t)new_size, sizeof(idx_t));
    if (!spf) {
        free(new_cache);
        exit(1);
    }

    /* λ(1) = 1 */
    new_cache[1] = 1;

    /* Compute spf up to n */
    for (idx_t i = 2; i < new_size; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = i;
            if (i * i <= n) {
                for (idx_t j = i * i; j < new_size; j += i)
                    if (spf[j] == 0)
                        spf[j] = i;
            }
        }
    }

    /* Compute λ using λ(m) = -λ(m / spf(m)) for m ≥ 2 */
    for (idx_t i = 2; i < new_size; ++i) {
        new_cache[i] = -new_cache[i / spf[i]];
    }

    /* Replace old cache (if any) */
    free(spf);
    free(lambda_cache);
    lambda_cache = new_cache;
    lambda_cache_size = new_size;
}

/* Main function requested by the build system */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset 1 – behavior for n = 0 is irrelevant */
        return 0;

    ensure_lambda(n);

    int64_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += (int)lambda_cache[n / k];
    }

    return (seq_t)sum;   /* safe: λ values are ±1, so sum is within int64_t */
}
