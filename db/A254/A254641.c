/*
 * A254641  Third partial sums of seventh powers (A001015).
 * https://oeis.org/A254641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1+n)*(2+n)*(3+n)*(6 -6*n -20*n^2 +15*n^3 +25*n^4
 *            +9*n^5 +n^6)/720
 * Range:     u8 none, u16 n <= 4, u32 n <= 16, u64 n <= 161, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* off‑by‑one guard; sequence is defined for n ≥ 1 */

    seq_t nn   = (seq_t)n;
    seq_t n_sq = nn * nn;
    seq_t n_cu = n_sq * nn;
    seq_t n4   = n_cu * nn;
    seq_t n5   = n4   * nn;
    seq_t n6   = n5   * nn;

    /* polynomial part: 6 -6*n -20*n^2 +15*n^3 +25*n^4 +9*n^5 +n^6 */
    seq_t poly = n6;
    poly += (seq_t)9 * n5;
    poly += (seq_t)25 * n4;
    poly += (seq_t)15 * n_cu;
    poly -= (seq_t)20 * n_sq;
    poly -= (seq_t)6 * nn;
    poly += (seq_t)6;

    /* multiply the factors n, n+1, n+2, n+3 and the polynomial,
       reducing by the denominator 720 as we go to avoid overflow   */
    seq_t num   = 1;
    seq_t denom = (seq_t)720;

    seq_t g = gcd(nn, denom);
    denom /= g;
    num   *= nn / g;

    seq_t a1 = nn + 1;
    g = gcd(a1, denom);
    denom /= g;
    num   *= a1 / g;

    seq_t a2 = nn + 2;
    g = gcd(a2, denom);
    denom /= g;
    num   *= a2 / g;

    seq_t a3 = nn + 3;
    g = gcd(a3, denom);
    denom /= g;
    num   *= a3 / g;

    g = gcd(poly, denom);
    denom /= g;
    num   *= poly / g;

    /* denom should now be 1; final division is exact */
    return num / denom;
}
