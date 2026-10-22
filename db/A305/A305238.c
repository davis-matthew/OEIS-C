/*
 * A305238  Negative numbers in base -10.
 * https://oeis.org/A305238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 9090, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = base -10 representation of -n interpreted as a decimal integer. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not used for the OEIS offset */

    /* work with a signed 128‑bit integer to safely hold –n even for
       the largest 64‑bit index. */
    __int128 N = -( __int128)n;          /* N is negative */
    const __int128 base = -10;

    /* an array big enough for any 64‑bit n (the representation has
       at most 21 digits in base –10). */
    unsigned char digits[64];
    size_t len = 0;

    while (N != 0)
    {
        __int128 r = N % base;   /* remainder may be negative */
        if (r < 0) r += 10;       /* make it 0..9 */
        digits[len++] = (unsigned char)r;
        N = (N - r) / base;      /* next quotient */
    }

    /* convert the digit array (most significant first) to a decimal
       integer of type seq_t. */
    seq_t result = 0;
    for (size_t i = 0; i < len; ++i)
        result = result * 10 + (seq_t)digits[len - 1 - i];

    return result;
}
