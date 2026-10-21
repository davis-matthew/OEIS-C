/*
 * A247891  a(n) is Gray-coded in base 4 into n.
 * https://oeis.org/A247891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + ⎣n/4⎦ + ⎣n/16⎦ + … (base‑4 addition without carries)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Extract base‑4 digits of n (least significant first).   */
    uint8_t digits[65];
    size_t len = 0;
    idx_t temp = n;
    while (temp)
    {
        digits[len++] = temp & 3;
        temp >>= 2;            /* divide by 4 */
    }

    /* Compute suffix sums modulo 4 (this yields the Digits of a(n)). */
    uint8_t a_digits[65];
    uint8_t suffix = 0;
    for (size_t i = len; i > 0; --i)
    {
        suffix = (suffix + digits[i - 1]) & 3;
        a_digits[i - 1] = suffix;
    }

    /* Reconstruct a(n) from its base‑4 digits. */
    seq_t res = 0;
    seq_t pow = 1;            /* pow = 4^i for the current i     */
    for (size_t i = 0; i < len; ++i)
    {
        if (a_digits[i])
            res += (seq_t)a_digits[i] * pow;
        pow <<= 2;            /* multiply by 4 for the next digit */
    }

    return res;
}
