/*
 * A332202  Largest k >= 0 such that 3^k divides 2^(2^n-1) + 1.
 * https://oeis.org/A332202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(2n-1)=1; a(2n)=2+v3(n) (v3 is the exponent of 3 in n).
 *            So for even n>0: a(n)=2+v3(n/2); for odd n>0: a(n)=1; for n==0:0
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n==0 case */
    if (n == 0) {
        return (seq_t)0;
    }

    /* odd indices (except 0) */
    if (n & 1U) {
        return (seq_t)1;
    }

    /* even indices */
    idx_t m = n >> 1;          /* m = n/2 */
    unsigned int v3 = 0;       /* exponent of 3 in m */

    while (m % 3 == 0) {
        m /= 3;
        ++v3;
    }

    return (seq_t)(2 + v3);
}
