/*
 * A283351  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 619", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283351:
 * a(0) = 1
 * for n >= 1, let l = n if n is odd, else n-1;
 * a(n) = (10^l - 1) / 9
 * This yields: 1,1,1,111,111,11111,11111,1111111,1111111,...
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t len = n;
    if ((n & 1) == 0)   /* even n: use the previous odd length */
        len--;

    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < len; ++i)
        pow10 = pow10 * (seq_t)10;

    return (pow10 - (seq_t)1) / (seq_t)9;
}
