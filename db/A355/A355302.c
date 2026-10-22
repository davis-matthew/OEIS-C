/*
 * A355302  a(n) is the number of normal undulating integers that divide n.
 * https://oeis.org/A355302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = number of normal undulating divisors of n (a number is
 *            normal undulating if its decimal digits alternate strictly
 *            up/down with no equal adjacent digits)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether x is a normal undulating integer. */
static bool is_normal_undulating(uint64_t x)
{
    /* Single‑digit numbers are normal. */
    if (x < 10)
        return true;

    uint8_t prev = x % 10;
    x /= 10;
    int last_diff = 0;  /* 1 = increase, -1 = decrease, 0 = none yet */

    while (x > 0) {
        uint8_t cur = x % 10;
        if (cur == prev)
            return false;              /* equal adjacent digits */

        int diff = (cur > prev) ? 1 : -1;
        if (last_diff != 0 && diff == last_diff)
            return false;              /* two consecutive same sign */

        last_diff = diff;
        prev = cur;
        x /= 10;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    uint64_t N = n;          /* cast to a wide integer for calculations */

    /* Enumerate divisors by scanning up to the integer square root. */
    for (uint64_t d = 1; d * d <= N; ++d) {
        if (N % d == 0) {
            if (is_normal_undulating(d))
                ++count;
            uint64_t q = N / d;
            if (q != d && is_normal_undulating(q))
                ++count;
        }
    }
    return count;
}
