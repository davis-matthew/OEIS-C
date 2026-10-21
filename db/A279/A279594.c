/*
 * A279594  Coefficients in the expansion of 1/([r] + [2r]x + [3r]x^2 + ...);
 *          [ ] = floor, r = sqrt(6)/2.
 * https://oeis.org/A279594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) are the coefficients of 1 / (⌊r⌋+⌊2r⌋x+⌊3r⌋x²+…), where r =
 *            sqrt(6)/2. The coefficients satisfy a(0) = 1 and a(n) =
 *            –∑_{k=1}^{n} ⌊(k+1) r⌋ · a(n–k) for n≥1
 * Range:     i8 n <= 36, i16 n <= 79, i32 n <= 161, i64 n <= 325,
 *            i128 n <= 654, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* --- Utility: integer square root of an unsigned __int128 value --- */
static unsigned long long isqrt_uint128(unsigned __int128 x)
{
    unsigned long long lo = 0;
    unsigned long long hi = (unsigned long long)-1;   /* 2⁶⁴–1 */
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 == x) return mid;
        if (mid2 < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;   /* floor(sqrt(x)) */
}

/* --- floor_r(n) = ⌊ r·n ⌋ ,  r = sqrt(6)/2,  n ≥ 0 --- */
static seq_t floor_r_idx(idx_t n)
{
    /* r = sqrt(6)/2 ⇒ r·n = sqrt(6·n²)/2.
       Compute floor(sqrt(6·n²)) with integer arithmetic. */
    unsigned __int128 sq = (unsigned __int128)6 * n * n;
    unsigned long long root = isqrt_uint128(sq);
    return (seq_t)(root / 2u);   /* divide by 2, integer division */
}

/* --- Sequence cache ------------------------------------------------- */
static seq_t *a_cache = NULL;      /* allocated a[0 .. cache_size-1] */
static idx_t  cache_size = 0;      /* current length of a_cache   */
static idx_t  cache_computed = 0;  /* first index not yet computed */

/* --- Main sequence function ---------------------------------------- */
seq_t a(idx_t n)
{
    /* Expand cache if needed */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!tmp && new_size > 0) {
            /* Allocation failed – abort as the evaluation harness
               cannot continue. */
            exit(EXIT_FAILURE);
        }
        a_cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms in increasing order */
    for (idx_t i = cache_computed; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = 1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t ck = floor_r_idx(k + 1);       /* ⌊(k+1)·r⌋ */
            sum += ck * a_cache[i - k];          /* seq_t multiply chosen */
        }
        a_cache[i] = -sum;                       /* since ⌊r⌋ = 1 */
    }
    cache_computed = n + 1;

    return a_cache[n];
}
