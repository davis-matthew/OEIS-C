/*
 * A355439  Smallest Lucas number beginning with n.
 * https://oeis.org/A355439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest Lucas number whose decimal representation starts
 *            with the decimal representation of n
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 16, u64 n <= 35, u128 n <= 45,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"

/* Return the number of decimal digits of a seq_t value. */
static int seq_digit_len(seq_t x)
{
    if (x == 0) return 1;
    int len = 0;
    seq_t y = x;
    seq_t ten = (seq_t)10;
    while (y > 0) {
        y /= ten;
        ++len;
    }
    return len;
}

/* Check whether the decimal representation of x begins with the
   representation of target (which has tlen digits). */
static bool starts_with(seq_t x, seq_t target, int tlen)
{
    int len = seq_digit_len(x);
    if (len < tlen) return false;
    seq_t pow = pow10((idx_t)(len - tlen));
    return (x / pow) == target;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;
    int target_len = seq_digit_len(target);

    /* Base Lucas numbers */
    seq_t L0 = (seq_t)2;
    seq_t L1 = (seq_t)1;

    if (starts_with(L0, target, target_len)) return L0;
    if (starts_with(L1, target, target_len)) return L1;

    seq_t prev = L0;
    seq_t cur  = L1;
    for (;;) {
        seq_t next = prev + cur;
        if (starts_with(next, target, target_len)) return next;
        prev = cur;
        cur  = next;
    }
}
