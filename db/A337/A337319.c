/*
 * A337319  a(n) = Sum_{i = 1..floor(log_2(n))+1} g(frac(n/2^i)), where g(t) =
 *          [0 if t = 0, -1 if 0 < t < 1/2, 1 if t >= 1/2], and where frac(x)
 *          denotes the fractional part.
 * https://oeis.org/A337319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i = 1..floor(log_2(n))+1} g(frac(n/2^i)), where g(t)
 *            = 0 if t = 0, -1 if 0 < t < 1/2, +1 if t >= 1/2. The
 *            implementation scans the binary digits of n: For each bit
 *            position i−1, if the bit is 1 we add +1; if the bit is 0 and any
 *            lower bit is 1 we subtract 1; otherwise (all lower bits...
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;
    if (n == 0) /* sequence is defined for n >= 1, but be safe */
        return 0;

    idx_t pos = 0;
    while ((n >> pos) != 0) {          /* for all bits up to the highest one */
        /* check the bit at position pos (i-1 in the definition) */
        if ((n >> pos) & 1ULL) {
            result += 1;               /* g(t) = +1 if the bit is 1 */
        } else {
            /* lower bits below current position */
            idx_t lower = pos == 0 ? 0 : ((1ULL << pos) - 1);
            lower &= n;
            if (lower != 0)
                result -= 1;           /* g(t) = -1 if the bit is 0 and lower bits non‑zero */
            /* otherwise g(t) = 0 : nothing to add */
        }
        pos++;
    }
    return result;
}
