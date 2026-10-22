/*
 * A395303  a(n) is the least k > 0 such that A162439(k) = n; a(0) = 0.
 * https://oeis.org/A395303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) is obtained by concatenating the binary representations *
 *            (in decreasing power‑of‑two order) of the powers of two that sum
 *            to n. * Equivalent definition: a(0) = 0; for n > 0, write n in
 *            binary, * take the set bits from most significant to least, and
 *            build a new * binary number by concatenati...
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 250, u64 n <= 2045,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) = 0 */

    seq_t res = 0;

    /* Find index of the most significant set bit. */
    idx_t msb = 0;
    idx_t t = n;
    while (t > 1) {          /* t >>= 1; msb++; */
        t >>= 1;
        msb++;
    }

    /* Process bits from msb down to 0. */
    for (idx_t b = msb; ; --b)
    {
        if ((n >> b) & 1u)          /* bit b is set */
        {
            /* Append the binary representation of 2^b:
             * [b+1] bits long: a 1 followed by b zeros.
             */
            res <<= (b + 1);
            res |= ((seq_t)1 << b);
        }
        if (b == 0) break;          /* avoid underflow */
    }

    return res;
}
