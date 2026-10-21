/*
 * A260688  a(n) = the least number of pieces of currency of denominations
 *          .01, .05, .10, .25, 1, 5, 10, 20, 50, 100 that the greedy
 *          algorithm uses to make n times .01 (n "cents") in change.
 * https://oeis.org/A260688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of coins used by the greedy algorithm with
 *            denominations 1, 5, 10, 25, 100, 500, 1000, 2000, 5000, 10000
 *            (cents)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const idx_t _denoms[10] = {
    10000, /* $100 */
    5000,  /* $50  */
    2000,  /* $20  */
    1000,  /* $10  */
    500,   /*  $5  */
    100,   /*  $1  */
    25,    /* 25¢  */
    10,    /* 10¢  */
    5,     /* 5¢   */
    1      /* 1¢   */
};

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t rem = n;
    for (size_t i = 0; i < 10; ++i) {
        idx_t d = _denoms[i];
        if (rem < d) continue;
        idx_t q = rem / d;
        rem -= q * d;
        count += (seq_t)q;
        if (rem == 0) break;
    }
    return count;
}
