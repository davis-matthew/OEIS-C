/*
 * A343230  A binary encoding of the digits "0" in balanced ternary
 *          representation of n.
 * https://oeis.org/A343230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = binary number whose i‑th bit (i=0 for the least
 *            significant digit) is 1 iff the i‑th digit of the
 *            balanced‑ternary representation of n (ignoring the most
 *            significant digit) is zero
 * Range:     u8 n <= 6561, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Special case n = 0: representation “0” has no non‑leading zeros. */
    if (n == 0)
        return (seq_t)0;

    /* Balanced‑ternary digits, least‑significant first.  The maximum
       number of digits for a 64‑bit n is < 40, so a small array suffices. */
    signed char digits[64];
    unsigned int len = 0;
    idx_t tmp = n;

    while (tmp != 0) {
        idx_t r = tmp % 3;
        tmp /= 3;
        if (r == 0)
            digits[len++] = 0;
        else if (r == 1)
            digits[len++] = 1;
        else {            /* r == 2 → digit “T” = −1 */
            digits[len++] = -1;
            tmp += 1;     /* carry to next higher digit */
        }
    }

    /* Assemble the result from the non‑leading digits (all positions
       except the most significant one). */
    seq_t result = 0;
    for (unsigned int i = 0; i + 1 < len; ++i) {
        if (digits[i] == 0)
            result |= ((seq_t)1 << i);
    }
    return result;
}
