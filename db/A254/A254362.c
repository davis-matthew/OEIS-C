/*
 * A254362  a(n) = 3*2^n + 3^n + 6.
 * https://oeis.org/A254362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n + 3^n + 6
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n and 3^n by repeated multiplication.
       The type seq_t is either an unsigned integer type or a
       big‑integer class that supports the * operator. */
    seq_t pow2 = (seq_t)1;
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
        pow3 *= (seq_t)3;
    }
    /* a(n) = 3*2^n + 3^n + 6 */
    return pow3 + (pow2 * (seq_t)3) + (seq_t)6;
}
