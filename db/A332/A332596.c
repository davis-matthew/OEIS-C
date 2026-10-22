/*
 * A332596  Number of quadrilateral regions in a "frame" of size n X n (see
 *          Comments in A331776 for definition), divided by 8.
 * https://oeis.org/A332596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-1)*(n-4) * - Σ_{i=2}^{⌊n/2⌋} (n+1-i)*(2*n+2-7*i)*φ(i)
 *            * + Σ_{i=⌊n/2⌋+1}^{n} (n+1-i)*(2*n+2-i)*φ(i)) / 2, * a(1) = 0
 * Range:     u8 n <= 7, u16 n <= 35, u32 n <= 578, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Internal cache for φ values */
static size_t phi_cache_size = 0;
static uint64_t *phi_cache = NULL;

/* Rebuild the φ table up to n (inclusive) using the standard sieve. */
static void rebuild_phi_cache(idx_t n)
{
    if (phi_cache_size >= (size_t)n + 1)
        return;  /* already big enough */

    free(phi_cache);
    phi_cache_size = (size_t)n + 1;
    phi_cache = (uint64_t *)malloc(phi_cache_size * sizeof(uint64_t));
    if (!phi_cache)
        return;  /* memory error; the caller will crash later, which is OK */

    for (size_t i = 0; i < phi_cache_size; ++i)
        phi_cache[i] = i;

    for (size_t i = 2; i < phi_cache_size; ++i)
    {
        if (phi_cache[i] == i)  /* i is prime */
        {
            for (size_t j = i; j < phi_cache_size; j += i)
                phi_cache[j] -= phi_cache[j] / i;
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0; /* a(1) = 0 */

    /* Ensure φ values up to n are available */
    rebuild_phi_cache(n);

   /* Use long long for all intermediate calculations to keep signedness. */
    long long N = (long long)n;
    long long part1 = (N - 1) * (N - 4);

    long long sum1 = 0;
    long long sum2 = 0;

    idx_t half = n / 2;
    for (idx_t i = 2; i <= half; ++i)
    {
        long long ii   = (long long)i;
        long long phi_i = (long long)phi_cache[i];
        long long term  = (N + 1 - ii) * (2 * N + 2 - 7 * ii);
        sum1 += term * phi_i;
    }

    for (idx_t i = half + 1; i <= n; ++i)
    {
        long double dummy = 0; /* unused, just to silence warning in some compilers */
        long long ii   = (long long)i;
        long long phi_i = (long long)phi_cache[i];
        long long term  = (N + 1 - ii) * (2 * N + 2 - ii);
        sum2 += term * phi_i;
    }

    long long numerator = part1 - sum1 + sum2;
    seq_t result = (seq_t)(numerator / 2);
    return result;
}
