/*
 * A329462  Expansion of Sum_{k>=1} (-1 + Product_{j>=1} (1 + x^(k*j^2))).
 * https://oeis.org/A329462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A329462: a(n) = Sum_{d|n} A033461(d)
 *  where A033461(k) = number of partitions of k into distinct squares
 *
 *  Algorithm: compute A033461 up to the requested n using a DP over
 *  distinct squares, then compute a(n) by divisor summation.
 *  Caches are kept in static arrays and grown only when a larger n
 *  is requested.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc/realloc/abort/free */
#include <string.h>   /* memset */

/* Integer floor square root – works for any unsigned 64‑bit integer. */
static idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid)  /* mid*mid would overflow or > n */
            high = mid - 1;
        else
        {
            idx_t sq = mid * mid;
            if (sq == n) return mid;
            if (sq < n) low = mid + 1;
            else high = mid - 1;
        }
    }
    return high;   /* floor(sqrt(n)) */
}

/* Allocation helpers that work in both C and C++. */
#ifdef __cplusplus
static seq_t* alloc_array(size_t n)
{
    return new seq_t[n]();   /* value‑initialised */
}
static void free_array(seq_t* p)
{
    delete[] p;
}
#else
static seq_t* alloc_array(size_t n)
{
    return (seq_t*)calloc(n, sizeof(seq_t));  /* zeroed memory */
}
static void free_array(seq_t* p)
{
    free(p);
}
#endif

/* Cached data. */
static seq_t *dp      = NULL;   /* dp[t] = #partitions of t into distinct squares */
static seq_t *a_arr   = NULL;   /* a_arr[n] = a(n) */
static idx_t  max_computed = 0;
static size_t dp_size   = 0;
static size_t aarr_size = 0;

/* Recompute dp and a_arr up to new_max (inclusive). */
static void recompute_upto(idx_t new_max)
{
    if (new_max <= max_computed)
        return;

    /* Allocate or grow dp array. */
    if (dp_size < (size_t)new_max + 1)
    {
        free_array(dp);
        dp = alloc_array((size_t)new_max + 1);
        dp_size = (size_t)new_max + 1;
    }
    /* Zero dp[0..new_max]. */
    for (idx_t i = 0; i <= new_max; ++i)
        dp[i] = 0;

    dp[0] = 1;

    /* Distinct‑square partitions DP. */
    idx_t max_sq = _floor_isqrt_u64(new_max);
    for (idx_t s = 1; s <= max_sq; ++s)
    {
        idx_t sq = s * s;
        for (idx_t t = new_max; t >= sq; --t)
            dp[t] += dp[t - sq];
    }

    /* Allocate or grow a_arr array. */
    if (aarr_size < (size_t)new_max + 1)
    {
        free_array(a_arr);
        a_arr = alloc_array((size_t)new_max + 1);
        aarr_size = (size_t)new_max + 1;
    }
    /* Zero a_arr[0..new_max]. */
    for (idx_t i = 0; i <= new_max; ++i)
        a_arr[i] = 0;

    /* Divisor summation to compute a_arr. */
    for (idx_t d = 1; d <= new_max; ++d)
    {
        seq_t val = dp[d];
        for (idx_t m = d; m <= new_max; m += d)
            a_arr[m] += val;
    }

    max_computed = new_max;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* sequence defined only for n ≥ 1 */

    if (n > max_computed)
        recompute_upto(n);

    return a_arr[n];
}
