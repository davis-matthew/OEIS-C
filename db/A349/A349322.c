/*
 * A349322  a(n) = Sum_{d|n} d^c(d), where c is the characteristic function of
 *          refactorable numbers (A336040).
 * https://oeis.org/A349322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (d^c(d)), where c(d)=1 iff d is refactorable
 *            (i.e. the number of divisors of d divides d), otherwise c(d)=0.
 *            Equivalently: add d for each refactorable divisor d of n, else
 *            add 1
 * Range:     u8 n <= 179, u16 n <= 9892, u32 n <= 9892, u64 n <= 9892,
 *            u128 n <= 9892, big any n
 * Helpers:   _floor_isqrt_u64, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, return 0 safely */
    seq_t sum = 0;
    idx_t s = _floor_isqrt_u64(n);

    for (idx_t d = 1; d <= s; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;

            /* process divisor d */
            uint64_t tau_d = divisor_count_u64(d);
            if (d % tau_d == 0)
                sum += (seq_t)d;
            else
                sum += (seq_t)1;

            /* process paired divisor d2 if distinct */
            if (d2 != d) {
                uint64_t tau_d2 = divisor_count_u64(d2);
                if (d2 % tau_d2 == 0)
                    sum += (seq_t)d2;
                else
                    sum += (seq_t)1;
            }
        }
    }
    return sum;
}
