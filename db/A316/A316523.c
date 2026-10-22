/*
 * A316523  Number of odd multiplicities minus number of even multiplicities
 *          in the canonical prime factorization of n.
 * https://oeis.org/A316523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{p^e‖n} (-1)^(e+1) (a(1)=0)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return 0;
    }

    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* odd exponent contributes +1, even exponent contributes -1 */
        if ((ex[i] & 1) != 0) {
            sum += 1;
        } else {
            sum -= 1;
        }
    }
    return sum;
}
