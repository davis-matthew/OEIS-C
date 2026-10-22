/*
 * A306436  Rewrite n by substituting each digit d of n by d-1 if d is odd,
 *          d+1 otherwise.
 * https://oeis.org/A306436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by swapping decimal digits 0↔1, 2↔3, 4↔5, 6↔7,
 *            8↔9
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t and standard integer types */

seq_t a(idx_t n)
{
    /* Special case for n = 0: its only digit 0 maps to 1. */
    if (n == 0)
        return (seq_t)1;

    seq_t result = 0;   /* accumulator for the transformed number */
    seq_t factor = 1;   /* 10^position of the digit being processed */

    idx_t tmp = n;
    while (tmp > 0) {
        unsigned digit = tmp % 10;      /* extract least‑significant decimal digit */
        unsigned new_digit = digit ^ 1; /* toggle each pair: 0↔1, 2↔3, …, 8↔9 */

        result += (seq_t)new_digit * factor; /* add the transformed digit */
        factor *= (seq_t)10;                  /* move to next decimal position */
        tmp /= 10;                            /* remove processed digit */
    }

    return result;
}
