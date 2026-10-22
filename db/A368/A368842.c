/*
 * A368842  a(n) gives the number of triples of equally spaced equal digits in
 *          the binary expansion of n (without leading zeros).
 * https://oeis.org/A368842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = count of triples of equally spaced equal bits in the
 *            binary representation of n (without leading zeros)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static inline size_t bitlen(idx_t n)
{
    size_t len = 0;
    while (n)
    {
        ++len;
        n >>= 1;
    }
    return len ? len : 1;   /* for n == 0, treat length as 1 ("0") */
}

seq_t a(idx_t n)
{
    if (n == 0)               /* binary "0" has no triples */
        return 0;

    size_t len = bitlen(n);
    seq_t count = 0;

    /* distance between consecutive bits in the triple (d >= 1) */
    for (size_t d = 1; d <= (len - 1) / 2; ++d)
    {
        /* starting position of the first bit of the triple */
        size_t pmax = len - 1 - 2 * d;
        for (size_t p = 0; p <= pmax; ++p)
        {
            uint64_t b1 = (n >> p) & 1ULL;
            uint64_t b2 = (n >> (p + d)) & 1ULL;
            if (b1 != b2)                /* bits must be equal */
                continue;
            uint64_t b3 = (n >> (p + 2 * d)) & 1ULL;
            if (b2 == b3)                 /* all three equal */
                ++count;
        }
    }
    return count;
}
