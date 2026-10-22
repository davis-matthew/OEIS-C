/*
 * A338434  Sum of the numbers less than or equal to sqrt(n) whose square does
 *          not divide n.
 * https://oeis.org/A338434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor(sqrt(n))} k if k*k does NOT divide n
 * Range:     u8 n <= 529, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/int_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t m = int_isqrt_u64(n);
    seq_t sum = 0;
    for (idx_t k = 1; k <= m; ++k) {
        idx_t k2 = k * k;
        if (n % k2 != 0) {
            sum += (seq_t)k;
        }
    }
    return sum;
}
