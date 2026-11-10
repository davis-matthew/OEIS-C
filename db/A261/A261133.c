/*
 * A261133  a(n) = Max{k from {1..n} | T(n,k) = A049831(n)}, where T(n,k) is
 *          the triangle defined at A049828.
 * https://oeis.org/A261133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max{k in 1..n : T(n,k) = max_j T(n,j)}, T(n,k) = sum of
 *            remainders in the Euclidean algorithm on (n,k) (A049828); found
 *            by direct search over k
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A261133_sum_remainders(seq_t n, seq_t k)
{
    seq_t sum = 0;
    seq_t x = n;
    seq_t y = k;
    while (y != 0) {
        seq_t r = x % y;
        sum += r;
        x = y;
        y = r;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t best = 0;
    seq_t best_k = 1;
    seq_t N = (seq_t)n;
    for (idx_t k = 1; k <= n; k++) {
        seq_t K = (seq_t)k;
        seq_t t = A261133_sum_remainders(N, K);
        if (t >= best) {
            best = t;
            best_k = K;
        }
    }
    return best_k;
}
