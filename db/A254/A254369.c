/*
 * A254369  a(n) = 15*2^n + 4^n + 5*3^n + 35.
 * https://oeis.org/A254369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*2^n + 4^n + 5*3^n + 35
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = (seq_t)1;
    seq_t pow3 = (seq_t)1;
    seq_t pow4 = (seq_t)1;

    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
        pow3 *= (seq_t)3;
        pow4 *= (seq_t)4;
    }

    return (seq_t)15 * pow2 + pow4 + (seq_t)5 * pow3 + (seq_t)35;
}
