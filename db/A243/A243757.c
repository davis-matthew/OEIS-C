/*
 * A243757  a(n) = Product_{i=1..n} A060904(i).
 * https://oeis.org/A243757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^( ⌊n/5⌋ + ⌊n/25⌋ + ⌊n/125⌋ + … )
 * Range:     u8 n <= 19, u16 n <= 29, u32 n <= 59, u64 n <= 119,
 *            u128 n <= 229, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Seq: largest power of 5 dividing n!  */

#include <stdint.h>   /* for uint64_t etc. */

/* small helper: compute 5^e in seq_t without relying on external utilities */
static seq_t pow5(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 5;
    return r;
}

seq_t a(idx_t n)
{
    /* count the exponent of 5 in n! */
    idx_t e = 0;
    for (idx_t d = 5; d <= n; d *= 5) {
        e += n / d;
        /* prevent overflow of d * 5, though for n up to 10⁴ this is safe */
        if (d > n / 5) break;
    }
    return pow5(e);
}
