/*
 * A262682  Even bisection of A262680.
 * https://oeis.org/A262682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9876, u16 n <= 9876, u32 n <= 9876, u64 n <= 9876,
 *            u128 n <= 9876, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = A262680(2*n)
    where A262680(k) = number of perfect squares encountered before reaching zero
    when starting at k and repeatedly replacing k by k - d(k),
    d(k) = number of divisors of k.
    The count includes the starting k if it's a square, but excludes the final zero.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Local helper: integer square root of an unsigned 64‑bit number          */
static inline idx_t _isqrt_u64(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 0xFFFFFFFFULL;            /* sqrt(2^64-1) < 2^32 */

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;

        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;                            /* floor(sqrt(n)) */
}

/* --------------------------------------------------------------------- */
/* Cache of divisor counts up to the largest value seen so far            */
static idx_t         _div_cache_max   = 0;   /* maximum k cached (inclusive) */
static uint32_t *     _div_cache       = NULL; /* _div_cache[i] = d(i) */

/* Ensure that divisor counts are available for all values up to m.       */
static void _ensure_divisor_cache(idx_t m)
{
    if (m <= _div_cache_max)
        return;                       /* already have sufficient data */

    /* Allocate or grow the cache */
    uint32_t *new_cache;
    if (_div_cache)
        new_cache = (uint32_t *)realloc(_div_cache, (m + 1) * sizeof(uint32_t));
    else
        new_cache = (uint32_t *)malloc((m + 1) * sizeof(uint32_t));

    if (!new_cache)
        exit(EXIT_FAILURE);          /* memory allocation failed */

    _div_cache = new_cache;

    /* Initialise to zero */
    for (idx_t i = 0; i <= m; ++i)
        _div_cache[i] = 0;

    /* Sieve to compute divisor counts */
    for (idx_t i = 1; i <= m; ++i)
        for (idx_t j = i; j <= m; j += i)
            _div_cache[j] += 1;

    _div_cache_max = m;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                */
seq_t a(idx_t n)
{
    idx_t start_k = 2 * n;                 /* initial k = 2n */
    _ensure_divisor_cache(start_k);        /* guarantee divisor table */

    idx_t k   = start_k;
    seq_t count = 0;                       /* number of squares seen */

    while (k > 0) {
        idx_t r = _isqrt_u64(k);           /* floor(sqrt(k)) */
        if ((__uint128_t)r * r == k)       /* k is a perfect square */
            count += 1;

        k -= (idx_t)_div_cache[k];         /* next k = k - d(k) */
    }

    return count;
}
