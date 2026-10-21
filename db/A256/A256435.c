/*
 * A256435  First differences of sums of two squares.
 * https://oeis.org/A256435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 8258, u32 n <= 8258, u64 n <= 8258,
 *            u128 n <= 8258, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A256435 – first differences of sums of two squares
         a(n) = A001481(n+1) – A001481(n)
   The implementation enumerates all sums a²+b² up to a chosen bound,
   keeps them sorted and unique, and returns the difference between
   successive entries.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------
   Helper: integer square root of a 64‑bit unsigned value.
   Uses binary search on the 32‑bit interval and 128‑bit arithmetic to
   avoid overflow.
   ---------------------------------------------------------------------- */
static uint64_t floor_sqrt_u64(uint64_t n)
{
    uint64_t lo = 0, hi = ((uint64_t)1u << 32) - 1;   /* 2³²‑1 is sqrt(2⁶⁴‑1) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ----------------------------------------------------------------------
   Comparison function required by qsort for seq_t values.
   Works for built‑in integer types as well as arbitrary‑precision
   integer classes that provide operator< and operator>.
   ---------------------------------------------------------------------- */
static int compare_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    return (*a < *b) ? -1 : ((*a > *b) ? 1 : 0);
}

/* ----------------------------------------------------------------------
   Static cache of unique, sorted sums of two squares.
   sums[0] corresponds to the 0th element of A001481 (which is 0).
   ---------------------------------------------------------------------- */
static seq_t   *sums     = NULL;   /* array of values */
static size_t   sums_len = 0;       /* number of cached values   */
static uint64_t max_val  = 0;       /* numeric bound used for      */
                                   /* generating the current array */

/* ----------------------------------------------------------------------
   Generate all unique sums a²+b² that do not exceed the given bound.
   The generated values are sorted increasingly and stored in the
   global array 'sums'.  The previous array, if any, is freed.
   ---------------------------------------------------------------------- */
static void build_sums(uint64_t bound)
{
    uint64_t m = floor_sqrt_u64(bound);
    /* maximum number of unordered pairs (a,b) with 0≤a≤b≤m */
    size_t max_pairs = (size_t)(m + 1) * (m + 2) / 2;

    /* Temporary array to collect all sums */
    seq_t *tmp = (seq_t *)malloc(max_pairs * sizeof(seq_t));
    if (!tmp) abort();    /* out of memory */

    size_t count = 0;
    for (uint64_t a = 0; a <= m; ++a) {
        uint64_t a2 = a * a;
        for (uint64_t b = a; b <= m; ++b) {
            uint64_t s = a2 + b * b;
            if (s > bound) continue;          /* stay within the bound */
            tmp[count++] = (seq_t)s;
        }
    }

    /* Sort the temporary array */
    qsort(tmp, count, sizeof(seq_t), compare_seq_t);

    /* Remove duplicates in place */
    size_t out = 0;
    for (size_t i = 0; i < count; ++i) {
        if (out == 0 || tmp[i] != tmp[out - 1]) {
            tmp[out++] = tmp[i];
        }
    }

    /* Allocate a new array of the unique sums */
    seq_t *new_arr = (seq_t *)malloc(out * sizeof(seq_t));
    if (!new_arr) abort();
    for (size_t i = 0; i < out; ++i) new_arr[i] = tmp[i];

    /* Clean up */
    free(tmp);
    if (sums) free(sums);

    sums     = new_arr;
    sums_len = out;
    max_val  = bound;
}

/* ----------------------------------------------------------------------
   Ensure that the cache contains at least (n+1) values:
   sums[0] .. sums[n]  (index 0 corresponds to A001481(0)).
   If not, repeatedly double the numeric bound and regenerate the
   cache until the requirement is satisfied.
   ---------------------------------------------------------------------- */
static void ensure_sums(idx_t n)
{
    if (sums_len > (size_t)n) return;      /* already sufficient */

    /* Initial bound if this is the first call */
    if (max_val == 0) max_val = 1;

    while (sums_len <= (size_t)n) {
        /* double the bound, but stop when it overflows */
        if (max_val > UINT64_MAX / 2) max_val = UINT64_MAX;
        else                          max_val <<= 1;

        build_sums(max_val);
    }
}

/* ----------------------------------------------------------------------
   Public function implementing the OEIS sequence.
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n = 0 */

    ensure_sums(n);                /* guarantees sums[0..n] exist */

    /* The difference of consecutive terms of A001481 */
    seq_t diff = sums[(size_t)n] - sums[(size_t)(n - 1)];
    return diff;
}
