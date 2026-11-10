/*
 * A381966  Row sums of A381965.
 * https://oeis.org/A381966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{k=0..A031346(n)} A381965(n,k): n plus all iterates
 *            of the digit-product map until a value < 10 is reached
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9612 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t A381966_digit_product(seq_t x)
{
    seq_t p = 1;
    if (x == 0) return 0;
    while (x > 0) {
        seq_t d = x % 10;
        if (d == 0) return 0;
        p *= d;
        x /= 10;
    }
    return p;
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t sum = cur;
    while (cur >= 10) {
        cur = A381966_digit_product(cur);
        sum += cur;
    }
    return sum;
}
