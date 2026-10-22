/*
 * A378562  a(n) is the number of steps to reach 1 by reversing digits of n in
 *          the lowest base where reversal reduces the number.
 * https://oeis.org/A378562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of steps needed to reach 1 by repeatedly reversing
 *            the digits of the current value in the smallest base for which
 *            the reversal is strictly smaller than the current value
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t (used by idx_t, seq_t types) */

static seq_t rev_num(idx_t n, idx_t b)
{
    seq_t rev = 0;
    while (n > 0) {
        idx_t digit = n % b;
        rev = rev * (seq_t)b + (seq_t)digit;
        n /= b;
    }
    return rev;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    idx_t steps = 0;
    idx_t curr = n;
    while (curr > 1) {
        idx_t base = 2;
        seq_t r;
        for (;;) {
            r = rev_num(curr, base);
            if (r < curr) break;
            ++base;
        }
        curr = (idx_t)r;
        ++steps;
    }
    return (seq_t)steps;
}
