/*
 * A286146  Lower triangular region of square array A286101.
 * https://oeis.org/A286146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = the smallest integer satisfying
 *            r*(r+1)/2 >= n (row index) * k = n - r*(r-1)/2 (column index) *
 *            T(r,k) = (1/2)*(2 + ((gcd(r,k)+lcm(r,k))^2) - gcd(r,k) -
 *            3*lcm(r,k))
 * Range:     u8 n <= 13, u16 n <= 208, u32 n <= 8771, u64 n <= 8771,
 *            u128 n <= 8771, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute greatest common divisor of two seq_t values */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* n should be >= 1; this case is unused */

    /* Find smallest r such that r*(r+1)/2 >= n */
    idx_t low = 0;
    idx_t high = 1;
    /* exponentially increase high until it satisfies the inequality */
    while ((__int128)high * (high + 1) / 2 < n) high <<= 1;

    /* Binary search between low+1 and high */
    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if ((__int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t r = high;   /* row number (1‑based) */

    /* Number of terms before row r */
    __int128 prev128 = (__int128)(r - 1) * r / 2;
    idx_t  prev = (idx_t)prev128;

    idx_t k = n - prev;   /* column number (1‑based) */

    /* Convert to seq_t for arithmetic */
    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)k;

    /* Compute gcd and lcm */
    seq_t g = gcd_seq(rn, kn);
    seq_t l = (rn / g) * kn;      /* lcm = r/g * k */

    /* Compute the triangle entry using seq_t arithmetic */
    seq_t sum = g + l;
    seq_t term = (seq_t)2 + sum * sum - g - (seq_t)3 * l;
    term /= (seq_t)2;              /* final division by 2 */

    return term;
}
