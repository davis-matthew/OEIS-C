/*
 * A364169  Smallest integer m = b*c which satisfies (b + c)*n = m - 1.
 * https://oeis.org/A364169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = (b+c)*n + 1 where b*c = m and (b+c)*n = m-1 *
 *            Implementation: transform to (b-n)(c-n) = n² + 1. * For each n ≥
 *            1 find positive divisors x,y of n²+1 with x ≤ y and x ≤ n. *
 *            Minimize x + y; then m = (n+x)(n+y) = 2n² + n(x + y) + 1
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1639, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 1) /* special case */
        return 6; /* 2*3, (2+3)*1 = 6-1 */

    seq_t seq_n = (seq_t)n;
    seq_t D = seq_n * seq_n + 1;               /* n² + 1 */
    idx_t limit = (idx_t)floor_sqrt(D);        /* floor √(n²+1) */
    if (limit > n) limit = n;                  /* only need x ≤ n */
    seq_t best_sum = D + 1;                    /* initial large value */

    for (idx_t d = 1; d <= limit; ++d) {
        if (D % d == 0) {
            /* d is a divisor, other = D/d */
            if (d <= n) {
                seq_t x = (seq_t)d;
                seq_t y = D / d;
                seq_t sum = x + y;
                if (sum < best_sum)
                    best_sum = sum;
            }
        }
    }

    seq_t m = 2 * seq_n * seq_n + seq_n * best_sum + 1;
    return m;
}
