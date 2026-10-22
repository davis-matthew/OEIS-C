/*
 * A356384  For any n >= 0, let x_n(1) = n, and for any b > 1, x_n(b) =
 *          x_n(b-1) minus the sum of digits of x_n(b-1) in base b; a(n) is
 *          the least b such that x_n(b) = 0.
 * https://oeis.org/A356384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 266, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1.
   For n > 0 let x = n.
   For b = 2, 3, … compute x := x – sum_of_digits(x, base b);
   Return the first b for which x becomes 0. */
#include <stdint.h>
#include <stdbool.h>

static inline seq_t digit_sum(seq_t x, idx_t base)
{
    seq_t sb = (seq_t)base;
    seq_t sum = 0;
    while (x > 0) {
        sum += x % sb;
        x /= sb;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t x = (seq_t)n;
    for (idx_t b = 2; ; ++b) {
        x -= digit_sum(x, b);
        if (x == 0) return (seq_t)b;
    }
}
