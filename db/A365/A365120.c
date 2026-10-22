/*
 * A365120  G.f. A(x) satisfies A(x) = (1 + x / (1 - x*A(x))^2)^2.
 * https://oeis.org/A365120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*(n-k+1), k) * binomial(n+k-1,
 *            n-k) / (n-k+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t binom_value(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;       /* use symmetry */
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num   = n - k + i;          /* numerator of the i-th fraction */
        seq_t g1    = gcd(result, (seq_t)i); /* cancel part of i with current result */
        result /= g1;
        seq_t i2   = (seq_t)i / g1;
        seq_t g2   = gcd(num, i2);          /* cancel remaining part of i with num */
        num   /= g2;
        i2    /= g2;                       /* i2 should now be 1 */
        result *= num;                     /* multiply remaining numerator */
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* first binomial: binomial(2*(n-k+1), k) */
        seq_t bb1 = binom_value(2 * (n - k + 1), k);

        /* second binomial: binomial(n+k-1, n-k)
           Handle the n=0, k=0 case explicitly */
        seq_t bb2;
        if (n == 0 && k == 0) {
            bb2 = 1;
        } else if ((n + k - 1) < (n - k)) {
            bb2 = 0;
        } else {
            bb2 = binom_value(n + k - 1, n - k);
        }

        /* divide by (n-k+1) */
        seq_t denom = (seq_t)(n - k + 1);
        seq_t term = (bb1 * bb2) / denom;
        sum += term;
    }
    return sum;
}
