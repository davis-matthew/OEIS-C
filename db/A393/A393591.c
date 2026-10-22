/*
 * A393591  Greatest integer b such that the sum of the integer digits of n in
 *          bases 2..b strictly increases.
 * https://oeis.org/A393591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = greatest integer b such that the sequence of digit sums
   of n in bases 2,3,…,b is strictly increasing.
   Computed by testing successive bases until the increasing property fails. */

#include <stdint.h>

static seq_t digit_sum(idx_t n, idx_t base)
{
    seq_t sum = 0;
    idx_t tmp  = n;
    while (tmp) {
        sum += (seq_t)(tmp % base);
        tmp  /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* not used, only for safety */

    seq_t prev = digit_sum(n, 2);
    for (idx_t base = 3; ; ++base) {
        seq_t cur = digit_sum(n, base);
        if (cur <= prev)
            return (seq_t)base - 1;
        prev = cur;
    }
}
