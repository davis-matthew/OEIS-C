/*
 * A260860  Base-60 representation of a(n) is the concatenation of the base-60
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the base‑60 representations of 1, 2, …,
 *            n, n‑1, …, 2, 1 (with a(0)=0). The value returned is the integer
 *            represented by that base‑60 digit string
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t result = 0;

    /* Build the ascending part: 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t shift = 1;
        idx_t t = i;
        /* shift = 60^digits(i) */
        do {
            shift *= 60;
            t /= 60;
        } while (t > 0);
        result = result * shift + (seq_t)i;
    }

    /* Build the descending part: n‑1 .. 1 */
    for (idx_t i = n - 1; i >= 1; --i) {
        seq_t shift = 1;
        idx_t t = i;
        do {
            shift *= 60;
            t /= 60;
        } while (t > 0);
        result = result * shift + (seq_t)i;
    }

    return result;
}
