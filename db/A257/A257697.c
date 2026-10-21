/*
 * A257697  a(n)=0 for n <= 1; for n >= 2, a(n) = largest number that can be
 *          obtained by rotating non-msb bits of binary expansion of n (with
 *          A080541 or A080542), without the most significant bit of n: a(n) =
 *          A053645(A256999(n)).
 * https://oeis.org/A257697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A053645(A256999(n))
 * Range:     u8 n <= 512, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

/* Here a(n) is the maximum integer obtained by rotating the lower bits of n
   (excluding the most significant bit). */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t nn = (seq_t)n;
    unsigned bits = bitlen_seq_t(nn);   /* number of bits in n */
    unsigned len  = bits - 1;           /* length of the suffix to rotate */

    if (len == 0)        /* covers n = 1, already returned, but keep safe */
        return 0;

    seq_t mask = ((seq_t)1 << len) - 1;
    seq_t r    = nn & mask;           /* lower len bits of n */

    if (r == 0)        /* all suffix bits are 0 */
        return 0;

    seq_t best = r;

    for (unsigned k = 1; k < len; ++k)
    {
        seq_t rot = ((r << k) | (r >> (len - k))) & mask;
        if (rot > best)
            best = rot;
    }

    return best;
}
