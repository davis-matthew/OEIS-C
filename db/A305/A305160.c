/*
 * A305160  a(n) = 123*2^n - 99.
 * https://oeis.org/A305160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 123*2^n - 99
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;

    /* compute 2^n by repeated multiplication; safe for unsigned and big‑int types */
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
    }

    /* 123*2^n - 99 */
    seq_t res = (seq_t)123 * pow2 - (seq_t)99;
    return res;
}
