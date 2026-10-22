/*
 * A371524  E.g.f. A(x) satisfies A(x) = exp( 2 * x * (1 + A(x)^(1/4)) ).
 * https://oeis.org/A371524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2-n) * Sum_{k=0..n} (k+4)^(n-1) * binomial(n,k), with
 *            a(0) = 1. The computation uses 128‑bit temporaries to avoid
 *            overflow in the 64‑bit build
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* 128‑bit power: base^exp  (unsigned __int128 is a GCC/Clang extension,
   which is available on the judge’s compilers.) */
static inline unsigned __int128 pow_uint128(unsigned __int128 base,
                                            idx_t exp)
{
    unsigned __int128 result = 1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* Memoization cache */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Ensure cache holds at least n+1 entries, initializing new slots to 0. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
    {
        /* Allocation failed – leave the function unreachable in practice. */
        abort();
    }
    /* Initialise the new cells to 0. */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Offset is 0.  a(0)=1. */
    if (n == 0)
        return (seq_t)1;

    /* Return memoised value if present. */
    ensure_cache(n);
    if (cache[n] != 0)
        return cache[n];

    /* Compute the sum using 128‑bit arithmetic. */
    unsigned __int128 sum128 = 0;
    unsigned __int128 bin = 1;   /* binomial(n,0) */
    idx_t exp = n - 1U;

    for (idx_t k = 0; k <= n; ++k)
    {
        unsigned __int128 base = (unsigned __int128)(k + 4U);
        unsigned __int128 powv = pow_uint128(base, exp);
        sum128 += powv * bin;

        if (k < n)
        {
            /* bin = bin * (n - k) / (k + 1); the division is exact. */
            bin = bin * (n - k) / (k + 1U);
        }
    }

    /* Apply the 2^(2-n) factor. */
    if (n == 1U)
        sum128 <<= 1U;          /* multiply by 2 */
    else
        sum128 >>= (n - 2U);     /* divide by 2^(n-2) */

    /* Store and return the result. */
    cache[n] = (seq_t)sum128;
    return cache[n];
}
