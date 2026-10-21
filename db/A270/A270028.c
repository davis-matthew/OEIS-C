/*
 * A270028  a(n) is the smallest b >= 3 for which the base-b representation of
 *          n contains at least one 1 (or 0 if no such base exists).
 * https://oeis.org/A270028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest b ≥ 3 such that the base‑b representation of
 *            n contains at least one digit 1; returns 0 if no such base
 *            exists (n = 2)
 * Range:     u8 n <= 5097, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;   /* 1 in base 3 is “1”. */
    if (n == 2) return 0;          /* no base b ≥ 3 gives a 1 for 2. */

    idx_t b;
    for (b = 3; b <= n; ++b) {     /* base n always gives “10” → contains 1. */
        seq_t temp = (seq_t)n;
        seq_t bb   = (seq_t)b;
        bool has_one = false;

        while (temp > 0) {
            if ((temp % bb) == 1) {
                has_one = true;
                break;
            }
            temp /= bb;
        }
        if (has_one)
            return (seq_t)b;
    }
    return 0; /* unreachable for n ≥ 3 */
}
