/*
 * A279769  Numbers n such that the sum of digits of 9n is 18.
 * https://oeis.org/A279769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer k such that the sum of decimal
 *            digits of 9*k equals 18. This is computed by a simple exhaustive
 *            search
 * Range:     u8 n <= 130, u16 n <= 9988, u32 n <= 9988, u64 n <= 9988,
 *            u128 n <= 9988, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t digit_sum_idx(idx_t x)
{
    idx_t sum = 0;
    while (x) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1; 0 is undefined */

    idx_t count     = 0;
    idx_t candidate = 1;

    while (1) {
        /* compute digital sum of 9 * candidate */
        idx_t ds = digit_sum_idx(candidate * 9);

        if (ds == 18) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }

        ++candidate;
    }
}
