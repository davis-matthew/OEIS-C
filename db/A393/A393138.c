/*
 * A393138  Triangle read by rows. T(n, k) = Sum_{i=0..k} C(k, i) * C(n - i,
 *          floor((n - i) / 2)).
 * https://oeis.org/A393138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 88, u32 n <= 274, u64 n <= 986,
 *            u128 n <= 3568, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A393138: Triangle read by rows.
   T(n, k) = Sum_{i=0..k} C(k, i) * C(n-i, floor((n-i)/2)).
   a(n) is the row‑wise enumeration of T with offset 0. */

#include <stdint.h>

/* ------------------------------------------------------------ */
/* Integer square root of a 128‑bit unsigned integer.  Result
   fits in 64 bits because we only call this with values up to
   8*n+1 where n <= UINT64_MAX. */
static idx_t isqrt_u128(__uint128_t x)
{
    idx_t lo = 0, hi = ((idx_t)1) << 33;           /* hi^2 > max x */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if ((__uint128_t)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ------------------------------------------------------------ */
/* Compute C(n, k) multiplicatively using seq_t.  Assumes k <= n. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;    /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t num = n - k + i;   /* numerator term */
        result = (result * (seq_t)num) / (seq_t)i;
    }
    return result;
}

/* ------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Find the row r and column k that correspond to index n. */
    __uint128_t discr = ( __uint128_t)8 * n + 1;
    idx_t sqrt_discr = isqrt_u128(discr);
    idx_t r = (sqrt_discr - 1) / 2;                /* floor((sqrt(8n+1)-1)/2) */

    idx_t prev_terms = r * (r + 1) / 2;             /* terms before row r */
    idx_t k = n - prev_terms;                      /* column within row */

    /* Compute T(r, k) = Sum_{i=0..k} C(k, i) * C(r-i, floor((r-i)/2)). */
    seq_t sum = 0;
    for (idx_t i = 0; i <= k; ++i) {
        seq_t c1 = binom_seq(k, i);
        idx_t remaining = r - i;
        idx_t half = remaining / 2;
        seq_t c2 = binom_seq(remaining, half);
        sum += c1 * c2;
    }
    return sum;
}
