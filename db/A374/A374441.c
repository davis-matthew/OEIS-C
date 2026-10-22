/*
 * A374441  Triangle read by rows: T(n, k) = binomial(n - floor(k/2),
 *          ceiling(k/2)) - binomial(n - ceiling(k/2), floor(k/2)).
 * https://oeis.org/A374441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r and k are the row and column of n in the
 *            triangle T(n,k) = binomial(n - ⌊k/2⌋, ⌈k/2⌉) - binomial(n -
 *            ⌈k/2⌉, ⌊k/2⌋) (triangle read by rows)
 * Range:     u8 n <= 72, u16 n <= 390, u32 n <= 1350, u64 n <= 4899,
 *            u128 n <= 9777, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External integer types supplied by the build system */
#include <stdint.h>
#include <stddef.h>

/* Helper: integer square root of a 64‑bit unsigned value.
   Returns ⌊√x⌋. */
static idx_t _isqrt_u64(idx_t x)
{
    idx_t res = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t bit = ((idx_t)1) << i;
        idx_t candidate = res | bit;
        /* use 128‑bit multiplication to avoid overflow */
        if ((unsigned __int128)candidate * candidate <= (unsigned __int128)x)
            res = candidate;
    }
    return res;
}

/* Helper: multiplicative binomial coefficient.
   Assumes the result fits in seq_t. */
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;             /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i; /* division is exact at each step */
    }
    return res;
}

/* Return the nth term of OEIS A374441 */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2  */
    /* Solve r^2 + r - 2n ≥ 0  →  r = floor((√(8n+1)-1)/2)   */
    unsigned __int128 tmp = (unsigned __int128)8 * n + 1;
    idx_t sqrt_val = _isqrt_u64((idx_t)tmp);
    idx_t r = (sqrt_val - 1) >> 1;

    /* Starting index of this row */
    idx_t row_start = r * (r + 1) / 2;

    /* Column index within the row */
    idx_t k = n - row_start;

    /* ⌊k/2⌋ and ⌈k/2⌉ */
    idx_t floork = k >> 1;            /* k/2 rounded down */
    idx_t ceilk  = (k + 1) >> 1;      /* k/2 rounded up */

    /* Compute the triangle entry T(r,k) */
    seq_t term1 = _binom(r - floork, ceilk);
    seq_t term2 = _binom(r - ceilk, floork);
    return term1 - term2;            /* term1 ≥ term2 always */
}
