/*
 * A291321  Numbers m with the property that shifting the rightmost digit of m
 *          to the left end multiplies the number by 8.
 * https://oeis.org/A291321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(2k-1) = 8*(10^(13k)-1)/79, a(2k) = 9*(10^(13k)-1)/79
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 4, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* not used, but keeps sequence well‑defined */

    /* Determine k for the formulas. */
    idx_t k = (n & 1) ? (n + 1) / 2 : n / 2;

    /* Compute (10^(13k) - 1) / 79 exactly.  pow10 returns a seq_t. */
    seq_t t = pow10(13 * k);
    t -= 1;
    t /= 79;

    /* Multiply by 8 for odd n or by 9 for even n. */
    return (n & 1) ? t * 8 : t * 9;
}
