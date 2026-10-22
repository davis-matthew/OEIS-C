/*
 * A350656  a(n) is the least value of the orbit of n under repeated
 *          application of A350229 (the sum of a number and its balanced
 *          ternary digits).
 * https://oeis.org/A350656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   digit_sum_balanced
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = least value in the orbit of n under the map
   f(x) = x + sum of the balanced ternary digits of x.  */
#include <stdint.h>
#include "../utils/digit_sum_balanced.h"

static inline seq_t f_val(seq_t v)
{
    long long s = digit_sum_balanced(v);
    if (s >= 0) {
        return v + (seq_t)s;
    } else {
        return v - (seq_t)(-s);   /* s is negative */
    }
}

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    seq_t minv = cur;

    seq_t tortoise = f_val(cur);
    seq_t hare = f_val(f_val(cur));

    if (tortoise < minv) minv = tortoise;
    if (hare < minv) minv = hare;

    while (tortoise != hare) {
        tortoise = f_val(tortoise);
        hare = f_val(f_val(hare));
        if (tortoise < minv) minv = tortoise;
        if (hare < minv) minv = hare;
    }

    return minv;
}
