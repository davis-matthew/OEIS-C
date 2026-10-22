/*
 * A347409  Longest run of halving steps in the trajectory from n to 1 in the
 *          Collatz map (or 3x+1 problem), or -1 if no such trajectory exists.
 * https://oeis.org/A347409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = longest consecutive halving steps in the Collatz
 *            trajectory from n to 1
 * Range:     u8 n <= 26, u16 n <= 6862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    seq_t cur = (seq_t)n;
    seq_t maxLen = 0;
    seq_t curLen = 0;
    /* safety counter to avoid infinite loops; unlikely needed for normal n */
    const idx_t SAFETY_LIMIT = 5000000ULL;
    idx_t steps = 0;

    while (cur != (seq_t)1) {
        if (steps++ > SAFETY_LIMIT) {
            /* Return -1 (max unsigned value) if a trajectory seems not to exist */
            return (seq_t)(-1);
        }
        if ((cur & 1) == 0) {          /* even: halving step */
            curLen++;
            cur >>= 1;                 /* divide by 2 */
        } else {                       /* odd: apply 3x+1 */
            if (curLen > maxLen) maxLen = curLen;
            curLen = 0;
            cur = cur * 3 + 1;
        }
    }
    /* account for trailing halving run ending at 1 */
    if (curLen > maxLen) maxLen = curLen;
    return maxLen;
}
