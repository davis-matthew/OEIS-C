/*
 * A254031  a(n) = 1*5^n + 2*4^n + 3*3^n + 4*2^n + 5*1^n.
 * https://oeis.org/A254031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1*5^n + 2*4^n + 3*3^n + 4*2^n + 5*1^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;
    seq_t pow3 = 1;
    seq_t pow4 = 1;
    seq_t pow5 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
        pow3 *= (seq_t)3;
        pow4 *= (seq_t)4;
        pow5 *= (seq_t)5;
    }
    seq_t res = pow5;                    /* 1*5^n */
    res += (seq_t)2 * pow4;              /* 2*4^n */
    res += (seq_t)3 * pow3;              /* 3*3^n */
    res += (seq_t)4 * pow2;              /* 4*2^n */
    res += (seq_t)5;                     /* 5*1^n */
    return res;
}
