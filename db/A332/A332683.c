/*
 * A332683  a(n) = Sum_{k=1..n, gcd(n, k) = 1} ceiling(n/k).
 * https://oeis.org/A332683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n, gcd(n,k)=1} ceil(n/k)
 * Range:     u8 n <= 52, u16 n <= 6598, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        if (gcd(N, (seq_t)k) == 1) {
            seq_t div = N / (seq_t)k;
            seq_t rem = N % (seq_t)k;
            seq_t ceil_val = div + (rem != 0 ? 1 : 0);
            sum += ceil_val;
        }
    }
    return sum;
}
