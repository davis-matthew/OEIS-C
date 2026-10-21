/*
 * A259580  Sum of numbers in the n-th antidiagonal of the reciprocity array
 *          of 2.
 * https://oeis.org/A259580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{m=1..n} ( ((n-1)*(m-1) + (gcd(n,m)-1)) / 2 +
 *            (gcd(n,m) <= 2 ? 2 : 0) ) (derived from the given definition)
 * Range:     u8 n <= 10, u16 n <= 64, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    seq_t total = 0;
    seq_t n_minus_1 = (seq_t)n - 1;
    for (idx_t m = 1; m <= n; ++m) {
        idx_t g = gcd_idx(n, m);
        seq_t prod = n_minus_1 * (seq_t)(m - 1) + (seq_t)(g - 1);
        seq_t part = prod / 2;
        if (g <= 2)
            part += 2;
        total += part;
    }
    return total;
}
