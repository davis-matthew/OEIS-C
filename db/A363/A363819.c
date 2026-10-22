/*
 * A363819  G.f. satisfies A(x) = (1 + x/A(x)^2)/(1 - x)^3.
 * https://oeis.org/A363819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n-1) * Sum_{k=1..n} binomial(3*k-1,k) *
 *            binomial(3*(2*k-1), n-k) / (3*k-1) + binomial(n+2,2) (a(0)=1)
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t num1 = binomial(3 * k - 1, k);
        seq_t num2 = binomial(3 * (2 * k - 1), n - k);
        seq_t prod = num1 * num2;
        seq_t term = prod / (3 * k - 1);
        sum += term;
    }
    seq_t base = binomial(n + 2, 2);
    seq_t sign = (n % 2 == 0) ? -1 : 1;
    return sign * sum + base;
}
