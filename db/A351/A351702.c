/*
 * A351702  In the balanced ternary representation of n, reverse the order of
 *          digits other than the most significant.
 * https://oeis.org/A351702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value obtained by reversing all balanced‑ternary digits
 *            of n except the most significant digit
 * Range:     u8 n <= 123, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function is independent of build‑specific utilities. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Trick for non‑negative n (idx_t is unsigned). */
    if (n == 0)
        return 0;

    /* Store the balanced ternary digits (least significant first).  */
    /* 3^40 ~ 1.2e19, so 128 digits are more than enough for any
       practical idx_t value within the time limits. */
    int8_t digits[128];
    size_t len = 0;
    idx_t temp = n;

    while (temp > 0) {
        idx_t r = temp % 3;
        temp /= 3;
        if (r == 2) {
            /* Convert remainder 2 to -1 with a carry. */
            r = -1;
            temp += 1;
        }
        digits[len++] = (int8_t)r;
    }

    /* The most significant digit is the last one stored. */
    int msd = digits[len - 1];

    /* Compute the new value:
       bits at positions 0..len-2 are reversed, then the MSB is added. */
    __int128 sres = 0;
    __int128 pow = 1;  /* 3^0 */

    /* lower digits reversed */
    for (size_t i = 0; i < len - 1; ++i) {
        int d = digits[len - 2 - i];
        sres += (__int128)d * pow;
        pow *= 3;
    }
    /* most significant digit */
    sres += (__int128)msd * pow;

    return (seq_t)sres;
}
