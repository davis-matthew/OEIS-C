/*
 * A303901  Triangle read by rows of coefficients in expansion of (3-2x)^n,
 *          where n is a nonnegative integer.
 * https://oeis.org/A303901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   Let T(n,k) = C(n,k) * 3^(n-k) * (-2)^k (0 ≤ k ≤ n). The n‑th
 *            term a(n) is T(r,k) where r is the row and k is the column in
 *            the flattened triangle: r = max { i | i(i+1)/2 ≤ n } , k = n -
 *            r(r+1)/2. This implements the direct evaluation of the
 *            coefficients of (3‑2x)^r, row‑major order. Initial value...
 * Range:     i8 n <= 10, i16 n <= 36, i32 n <= 123, i64 n <= 442,
 *            i128 n <= 1669, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*----- Utilities -----*/

/* Compute floor(sqrt(x)) for a 64‑bit unsigned integer. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (idx_t)1 << 32;           /* 2^32 > sqrt(2^64-1) */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Find the row index r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 cum = (__int128)mid * (mid + 1) / 2;
        if (cum <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Integer exponentiation by repeated squaring: base^exp. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result = result * b;
        b = b * b;
        exp >>= 1;
    }
    return result;
}

/* Binomial coefficient C(n,k) using multiplicative formula. */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t t = 1; t <= k; ++t) {
        res = res * (n - k + t);
        res = res / t;          /* exact division at each step      */
    }
    return res;
}

/*----- Sequence function -----*/

seq_t a(idx_t n)
{
    /* Find row r and column k */
    idx_t r = find_row(n);
    idx_t start = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - start;

    /* Coefficient: C(r,k) * 3^(r-k) * (-2)^k */
    seq_t coef = binom_seq(r, k);
    seq_t pow3 = pow_seq((seq_t)3, r - k);
    seq_t pow2 = pow_seq((seq_t)2, k);

    coef = coef * pow3 * pow2;
    if (k & 1) coef = -coef;      /* (-2)^k = (-1)^k * 2^k */

    return coef;
}
