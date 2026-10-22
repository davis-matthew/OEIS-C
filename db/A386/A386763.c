/*
 * A386763  a(n) = Sum_{k=0..n} 5^k * 3^(n-k) * binomial(n+k-1,k).
 * https://oeis.org/A386763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 5^k * 3^(n-k) * binomial(n+k-1,k) (OEIS
 *            A386763)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, pow3, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow3.h"
#include "../utils/power5.h"

seq_t a(idx_t n) {
    /* Special case n = 0: the summation would require binomial(-1,0) */
    if (n == 0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t N = n;
    for (idx_t k = 0; k <= N; ++k) {
        seq_t term = power5(k) * pow3(N - k) * binomial(N + k - 1, k);
        sum += term;
    }
    return sum;
}
