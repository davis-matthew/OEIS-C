/*
 * A382895  Divide n successively by its nonzero digits from most to least
 *          significant, updating the result at each step and skipping any
 *          digit that doesn't divide the current value exactly.
 * https://oeis.org/A382895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the process of successively dividing n by its nonzero
 * decimal digits, from most to least significant, skipping any digit that
 * does not divide the current value exactly. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    if (val == 0) return (seq_t)0;

    /* Extract decimal digits of the original value into an array.
       We store them least‑significant first to later process from most to least. */
    uint8_t digits[70];      /* 70 digits covers all possible seq_t sizes. */
    size_t len = 0;
    seq_t tmp = val;
    while (tmp > 0)
    {
        digits[len++] = (uint8_t)(tmp % (seq_t)10);
        tmp /= (seq_t)10;
    }

    /* Process digits from most significant to least. */
    for (int i = (int)len - 1; i >= 0; --i)
    {
        uint8_t d = digits[i];
        if (d == 0) continue;                /* Skip zero digits. */
        if (val % (seq_t)d == 0)             /* Does the current value divide exactly? */
            val /= (seq_t)d;                 /* Divide by that digit. */
    }

    return val;
}
