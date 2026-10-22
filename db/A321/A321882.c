/*
 * A321882  a(n) is the least base b > 1 such that the sum n + n can be
 *          computed without carry.
 * https://oeis.org/A321882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 683, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS: A321882
 * a(n) = the least base b > 1 such that n + n can be computed without carry in base b.
 * This is equivalent to: all digits d of n in base b satisfy 2*d < b.
 *
 * Implementation: brute‑force search over bases starting at 2. For each base, compute the
 * base‑b digits of n and the maximum digit. If 2*max_digit < base, return that base.
 * The existence of a solution is guaranteed: for any n > 0, base 2*n + 1 works.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 2;                /* a(0) = 2 by definition */

    seq_t n_val = (seq_t)n;
    idx_t limit = 2 * n + 1;    /* upper bound where solution is guaranteed */

    for (idx_t b = 2; ; ++b) {
        seq_t maxd = 0;
        seq_t temp = n_val;

        while (temp > 0) {
            seq_t d = temp % (seq_t)b;   /* digit of n in base b */
            if (d > maxd) maxd = d;
            temp /= (seq_t)b;
        }

        if (maxd * 2 < (seq_t)b)   /* no carry when doubling */
            return (seq_t)b;

        if (b > limit)            /* safety fallback, should not happen */
            return (seq_t)b;
    }
}
