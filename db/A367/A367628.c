/*
 * A367628  Sum of the divisors of n <= tau(n).
 * https://oeis.org/A367628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d <= τ(n)} d
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"   /* for divisor_count_u64 */

/* Computes the sum of the divisors of n that are less than or equal to τ(n). */
seq_t a(idx_t n)
{
    /* τ(n) : number of positive divisors of n */
    uint64_t tau = divisor_count_u64(n);

    seq_t sum = 0;
    idx_t i;

    for (i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if ((seq_t)d1 <= tau) sum += (seq_t)d1;
            if (d2 != d1 && (seq_t)d2 <= tau) sum += (seq_t)d2;
        }
    }

    return sum;
}
