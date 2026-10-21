/*
 * A244492  Triangle read by rows: T(n,k) (n>=2, 0 <= k <= n-2) =
 *          n!/(2^i*i!*k!), where k=n-2i (or 0 for entries with wrong parity).
 * https://oeis.org/A244492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle read by rows of T(n,k) = n! / (2^i * i! * k!),
 *            where k = n - 2*i (otherwise 0)
 * Range:     u8 n <= 22, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/binom.h"

/* Global prime cache used by triangle_value. */
static uint64_t *prime_cache = NULL;
static size_t   prime_count = 0;
static idx_t    prime_max   = 0;

/* Ensure prime_cache contains all primes <= limit. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_max)
        return;

    /* Free old cache */
    free(prime_cache);
    prime_cache = NULL;
    prime_count = 0;
    prime_max   = limit;

    if (limit < 2)
        return;

    size_t sz = (size_t)(limit + 1);
    bool *comp = (bool*)malloc(sz);
    if (!comp)
        return;
    memset(comp, 0, sz);

    prime_cache = (uint64_t*)calloc(sz, sizeof(uint64_t));
    if (!prime_cache) {
        free(comp);
        return;
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            prime_cache[cnt++] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    comp[j] = true;
            }
        }
    }
    free(comp);
    prime_count = cnt;
}

/* Return exponent of prime p in n! (Legendre's formula). */
static inline idx_t fact_exp(idx_t n, idx_t p)
{
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* Compute triangular entry T(N,k) as defined. */
static seq_t triangle_value(idx_t N, idx_t k)
{
    if (((N - k) & 1U) != 0)           /* parity mismatch */
        return 0U;

    idx_t i = (N - k) >> 1;

    /* for N < 2, only k = 0 and i = 0 leads to 1, but we never call with N < 2 */
    ensure_primes(N);

    seq_t result = 1;
    for (size_t pi = 0; pi < prime_count; ++pi) {
        idx_t p   = prime_cache[pi];
        if (p > N)
            break;
        idx_t e = fact_exp(N, p) - fact_exp(N - 2 * i, p) - fact_exp(i, p);
        if (p == 2)
            e -= i;                     /* account for the 2^i factor in the denominator */
        for (idx_t t = 0; t < e; ++t)
            result *= (seq_t)p;
    }
    return result;
}

/* Main sequence access routine. */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t start = r * (r + 1) / 2;      /* first index of this row */
    idx_t k = n - start;                /* column within row */
    idx_t N = r + 2;                    /* triangle index */
    return triangle_value(N, k);
}
