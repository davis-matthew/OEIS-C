/*
 * A335107  Least period of the length-n prefix of the Thue-Morse sequence
 *          A010060.
 * https://oeis.org/A335107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n <= 3; find i such that 3*2^i < n <= 6*2^i. Then
 *            if n <= 5*2^i return 3*2^i else return 5*2^i (This piecewise
 *            form gives the least period of the length‑n prefix of the
 *            Thue–Morse sequence.)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3)
        return (seq_t)n;

    idx_t pow = 1;                     /* pow = 2^i */
    while ((seq_t)n > (seq_t)6 * (seq_t)pow)
        pow <<= 1;                      /* increase i until 6*2^i >= n */

    if ((seq_t)n <= (seq_t)5 * (seq_t)pow)
        return (seq_t)3 * (seq_t)pow;   /* a(n) = 3*2^i */
    else
        return (seq_t)5 * (seq_t)pow;   /* a(n) = 5*2^i */
}
