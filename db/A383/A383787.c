/*
 * A383787  Largest number obtainable by either keeping each decimal digit d
 *          in n or replacing it with 9-d.
 * https://oeis.org/A383787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reconstruct the decimal representation of n by replacing
 *            each digit d with max(d, 9 - d) and forming the largest possible
 *            integer
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the off‑by‑one offset: a(1) corresponds to n == 1. */
    /* Extract decimal digits of n into a buffer. */
    uint8_t buf[20];          /* n <= 2^64-1, at most 20 decimal digits */
    size_t pos = 0;
    uint64_t t = (uint64_t)n;

    if (t == 0) {
        buf[pos++] = 0;
    } else {
        while (t > 0) {
            buf[pos++] = (uint8_t)(t % 10);
            t /= 10;
        }
    }

    /* Rebuild the number from most significant digit to least. */
    seq_t res = 0;
    for (size_t i = pos; i > 0; --i) {
        uint8_t d = buf[i - 1];
        uint8_t nd = d < 5 ? 9U - d : d;   /* choose the larger of d and 9-d */
        res = res * (seq_t)10 + (seq_t)nd;
    }

    return res;
}
