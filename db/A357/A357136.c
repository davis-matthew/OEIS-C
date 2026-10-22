/*
 * A357136  Triangle read by rows where T(n,k) is the number of integer
 *          compositions of n with alternating sum k = 0..n. Part of the full
 *          triangle A097805.
 * https://oeis.org/A357136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A357136
   Definition: a(n) is obtained by reading the triangle
   T(r,k) row by row, where T(r,k) is the number of integer
   compositions of r with alternating sum k.
   Formula for T(r,k):
     T(r,k) = 0                     if r%2 != k%2
     T(r,k) = binom(r-1, (r-k)/2)   otherwise
   The index n is flattened across the rows: 0 → T(0,0),
   1 → T(1,0), 2 → T(1,1), 3 → T(2,0), ...
*/

#include <stdint.h>
#include <math.h>

/* Euclidean gcd for the integer type seq_t. */
static inline seq_t gcd_seq(seq_t a, seq_t b) {
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Compute binomial coefficient C(n,m) using seq_t arithmetic.
   The algorithm keeps all intermediate values exact by canceling
   common factors with gcds. */
static seq_t binom_idx(idx_t n, idx_t m) {
    if (m > n) return (seq_t)0;
    if (m == 0 || m == n) return (seq_t)1;
    if (m > n - m) m = n - m;

    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= m; ++i) {
        seq_t num = (seq_t)(n - m + i);
        seq_t den = (seq_t)i;

        /* cancel common factors between numerator and denominator */
        seq_t g = gcd_seq(num, den);
        num /= g;
        den /= g;

        /* cancel remaining common factors between result and denominator */
        if (den > 1) {
            seq_t g2 = gcd_seq(res, den);
            res /= g2;
            den /= g2;
        }

        /* den should now be 1; multiply */
        res *= num;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Determine the row r and column k corresponding to the
       flattened index n.  The triangular number T(r) = r*(r+1)/2
       counts the number of entries up to row r-1. */
    if (n == 0) {
        return (seq_t)1;  /* T(0,0) */
    }

    /* Initial approximation using the inverse of the triangular
       number formula. */
    idx_t r = (idx_t)((sqrt((long double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);

    /* Correct possible rounding errors. */
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;
    while ((r * (r + 1)) / 2 > n) --r;

    idx_t k = n - (r * (r + 1)) / 2;  /* column within the row */

    if (r == 0) {  /* only element in row 0 */
        return (seq_t)1;
    }

    /* If parity does not match, the count is zero. */
    if ((r & 1) != (k & 1)) {
        return (seq_t)0;
    }

    idx_t m = (r - k) / 2;
    return binom_idx(r - 1, m);
}
