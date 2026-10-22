/*
 * A377153  a(n) = Sum_{k=0..n} binomial(k+5,5) * binomial(k,n-k)^2.
 * https://oeis.org/A377153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+5,5) * binomial(k,n-k)^2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"          /* for gcd(seq_t, seq_t) */

/* Helper: compute (a*b)/c without overflow (uses gcd reduction) */
static seq_t mul_div_seq(seq_t a, seq_t b, seq_t c) {
    seq_t g = gcd(a, c);
    a /= g;
    c /= g;
    g = gcd(b, c);
    b /= g;
    c /= g;
    return (a * b) / c;
}

/* Helper: binomial(n,5) for n>=0 (returns 0 for n<5) */
static seq_t binom5(idx_t n) {
    if (n < 5) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= 5; ++i) {
        seq_t num = n - 5 + i;
        res = mul_div_seq(res, num, (seq_t)i);
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t max_m = n / 2;              /* floor(n/2) */
    seq_t c = 1;                      /* binom(n-m,m) with m=0 */
    seq_t d = binom5(n + 5);           /* binom(n-m+5,5) with m=0 */

    for (idx_t m = 0; m <= max_m; ++m) {
        /* term = d * c * c */
        seq_t term = d * c * c;
        sum += term;

        if (m == max_m) break;

        /* compute next c: c_{m+1} = c * (n-2m)*(n-2m-1) / ((n-m)*(m+1)) */
        seq_t numC = (seq_t)(n - 2 * m) * (seq_t)(n - 2 * m - 1);
        seq_t denC = (seq_t)(n - m) * (seq_t)(m + 1);
        c = mul_div_seq(c, numC, denC);

        /* compute next d: d_{m+1} = d * (n-m) / (n-m+5) */
        seq_t numD = (seq_t)(n - m);
        seq_t denD = (seq_t)(n - m + 5);
        d = mul_div_seq(d, numD, denD);
    }
    return sum;
}
