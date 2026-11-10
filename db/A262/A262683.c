/*
 * A262683  Characteristic function for A182859.
 * https://oeis.org/A262683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - A000035(A036459(n)); A036459(n) = steps of iterating
 *            d(n) to reach 2 (a(1)=a(2)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 144 terms through n = 9529 (2026-11-07)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    uint64_t cur = (uint64_t)n;
    unsigned steps = 0;
    while (cur > 2) {
        cur = num_divisors(cur);
        steps++;
    }
    return (seq_t)(1u - (steps & 1u));
}
