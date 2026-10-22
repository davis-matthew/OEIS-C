/*
 * A317312  Multiples of 12 and odd numbers interleaved.
 * https://oeis.org/A317312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 12 * n, a(2n+1) = 2 * n + 1 (equivalent to a(n) = (7 + 5
 *            * (-1)^n) * n / 2)
 * Range:     u8 n <= 43, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert n/2 to seq_t as the base multiplier */
    seq_t half = (seq_t)(n >> 1);
    if ((n & 1) == 0)         /* even index: 2n case */
        return (seq_t)(12 * half);
    else                      /* odd index: 2n+1 case */
        return (seq_t)(2 * half + 1);
}
