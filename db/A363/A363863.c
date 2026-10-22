/*
 * A363863  Numbers expressible as j^2 - k^2, 1 <= k <= j-2 ("squares with a
 *          square hole").
 * https://oeis.org/A363863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = j^2 - k^2 where j - k >= 2, 1 <= k <= j-2 Equivalent: n
 *            is a multiple of 4 (>=8) or an odd composite having two distinct
 *            odd factors > 1
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool qualifies(seq_t m)
{
    /* ignore numbers less than 8 */
    if (m < 8) return false;

    /* even numbers: must be a multiple of 4 (>=8) */
    if ((m & 1) == 0) {
        if ((m % 4) == 0 && m != 4) return true;
        return false;
    }

    /* odd numbers: must have two distinct odd factors */
    /* assume m fits into idx_t for the tested range (n <= 10000) */
    idx_t v = (idx_t)m;
    /* find the smallest odd divisor >1 */
    for (idx_t d = 3; d * d <= v; d += 2) {
        if (v % d == 0) {
            idx_t b = v / d;
            if (d != b) return true;   /* distinct factors */
            else break;                 /* perfect square of a prime */
        }
    }
    /* no divisor found → prime or 1 */
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety, defined though not used */

    idx_t count = 0;
    seq_t curr = 1;

    while (1) {
        if (qualifies(curr)) {
            ++count;
            if (count == n) return curr;
        }
        ++curr;
    }
}
