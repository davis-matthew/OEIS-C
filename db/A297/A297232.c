/*
 * A297232  Up-variation of the base-11 digits of n; see Comments.
 * https://oeis.org/A297232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of all positive differences between consecutive
 *            digits of n when n is expressed in base 11
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the base case n==0 gracefully, though the sequence is defined for n>=1. */
    if (n == 0)
        return 0;

    /* Extract base‑11 digits, least‑significant first.  A 64‑bit integer needs at most 19 digits in base 11, so 64 is ample. */
    unsigned char digits[64];
    idx_t k = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[k++] = (unsigned char)(tmp % 11);
        tmp /= 11;
    }

    seq_t uv = 0;
    /* Iterate from most significant to least significant digit. */
    for (idx_t i = k; i-- > 1; ) {
        unsigned char high = digits[i];     /* more significant digit */
        unsigned char low  = digits[i-1];   /* next less significant digit */
        if (low > high)
            uv += (seq_t)(low - high);     /* add positive difference */
    }
    return uv;
}
