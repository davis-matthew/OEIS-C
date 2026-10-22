/*
 * A358351  Number of values of m such that m + (sum of digits of m) +
 *          (product of digits of m) is n.
 * https://oeis.org/A358351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = # { m : 1 ≤ m ≤ n , m + sumDigits(m) + prodDigits(m) == n } */
/* The algorithm runs in O(n) time, which is easily fast enough for n ≤ 10 000. */

#include <stdint.h>

static inline void digit_sum_prod(idx_t x, idx_t *sum, unsigned __int128 *prod)
{
    idx_t s = 0;
    unsigned __int128 p = 1;
    while (x > 0) {
        idx_t d = x % 10;
        s += d;
        p *= d;
        x /= 10;
    }
    *sum = s;
    *prod = p;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    seq_t count = (seq_t)0;
    for (idx_t m = 1; m <= n; ++m) {
        idx_t s;
        unsigned __int128 p;
        digit_sum_prod(m, &s, &p);

        /* product of digits may exceed idx_t; cast only when safe */
        idx_t f = m + s + (idx_t)p;
        if (f == n)
            ++count;
    }
    return count;
}
