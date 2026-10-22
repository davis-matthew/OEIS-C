/*
 * A335182  Sum of the refactorable divisors of n.
 * https://oeis.org/A335182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 179, u16 n <= 8632, u32 n <= 8632, u64 n <= 8632,
 *            u128 n <= 8632, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A335182
 *  Formula: a(n) = Σ_{d|n} d * [tau(d) divides d]
 *  where tau(d) is the number of divisors of d.
 *  The term a(1) = 1.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

static inline bool is_refactorable(uint64_t d) {
    uint64_t tau = divisor_count_u64(d);
    return d % tau == 0;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* offset is 1, but guard just in case */
    uint64_t nn = (uint64_t)n;
    seq_t sum = 0;

    for (uint64_t i = 1; i * i <= nn; ++i) {
        if (nn % i == 0) {
            uint64_t d1 = i;
            if (is_refactorable(d1)) sum += (seq_t)d1;

            uint64_t d2 = nn / i;
            if (d2 != d1 && is_refactorable(d2)) sum += (seq_t)d2;
        }
    }
    return sum;
}
