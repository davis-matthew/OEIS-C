/*
 * A364800  The number of iterations that n requires to reach 1 under the map
 *          x -> A356874(x).
 * https://oeis.org/A364800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(A356874(n)) + 1 for n >= 2, a(1) = 0; A356874(n) =
 *            Sum_{i in S} F_i where n = Sum_{i in S} 2^(i-1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9886 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A356874_step(idx_t n)
{
    idx_t f1 = 1, f2 = 1; /* F_pos, F_{pos+1} */
    idx_t sum = 0;
    while (n) {
        if (n & 1U) sum += f1;
        idx_t t = f1 + f2;
        f1 = f2;
        f2 = t;
        n >>= 1U;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    while (n > 1) {
        n = A356874_step(n);
        ++count;
    }
    return count;
}
