/*
 * A273161  a(n) = Sum_{k=1..n} C(n-k, floor((n-k)/k)).
 * https://oeis.org/A273161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} C(n-k, floor((n-k)/k))
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Multiplicative computation of binomial coefficient C(N, R).
   Assumes R <= N and returns 0 if R > N. */
static seq_t binom_multiplicative(idx_t N, idx_t R) {
    if (R > N) return 0;
    if (R > N - R) R = N - R;   /* Use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= R; ++i) {
        res = res * (seq_t)(N - R + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t N = n - k;              /* n-k */
        idx_t R = N / k;              /* floor((n-k)/k) */
        sum += binom_multiplicative(N, R);
    }
    return sum;
}
