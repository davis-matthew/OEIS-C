/*
 * A327667  a(n) is the least base >= 2 where n is the least number with its
 *          sum of digits.
 * https://oeis.org/A327667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   min_num_with_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the least base b ≥ 2 such that n is the smallest integer whose
   sum of digits in base b equals the sum of digits of n in base b.
   The search starts at b = 2 and stops when the condition is satisfied
   (which always happens for b = n + 1 for n ≥ 1 and for b = 2 when n = 0). */

#include <stdbool.h>
#include "../utils/min_num_with_sum.h"

/* Sum of the digits of x written in the given base. */
static seq_t sum_digits_base(seq_t x, unsigned base)
{
    seq_t sum = 0;
    seq_t b   = (seq_t)base;
    while (x > 0) {
        sum += x % b;
        x   /= b;
    }
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t n_val = (seq_t)n;
    for (idx_t b = 2; ; ++b) {
        unsigned ub = (unsigned)b;
        seq_t s = sum_digits_base(n_val, ub);
        if (min_num_with_sum(s, ub) == n_val)
            return (seq_t)b;
        /* The base n + 1 is guaranteed to work for all n, so the
           loop will always terminate before ever overflowing. */
    }
}
