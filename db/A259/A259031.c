/*
 * A259031  Smallest m such that |A259029(m)| = n.
 * https://oeis.org/A259031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=3, a(1)=1, a(2)=16; for n>=3, a(n)=192*4^(n-3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)16;

    idx_t k = n - 3;           /* exponent for base 4 */
    seq_t pow4 = 1;            /* 4^k */
    seq_t base  = (seq_t)4;

    while (k > 0) {
        if (k & 1U) pow4 *= base;
        base *= base;
        k >>= 1U;
    }

    return (seq_t)192 * pow4;
}
