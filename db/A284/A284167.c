/*
 * A284167  a(n) = Sum_{i=1..A000005(n)} d(n+k(i)), where d(t) is the number
 *          of divisors of t and k(i) is the i-th divisor of n.
 * https://oeis.org/A284167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..A000005(n)} d(n + k(i))
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors of each divisor‑shifted value n+d */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    uint64_t N = (uint64_t)n;
    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            /* divisor d */
            sum += (seq_t)divisor_count_u64(N + d);
            uint64_t d2 = N / d;
            if (d2 != d) {
                /* complementary divisor d2 */
                sum += (seq_t)divisor_count_u64(N + d2);
            }
        }
    }
    return sum;
}
