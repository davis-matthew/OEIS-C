/*
 * A280370  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 259", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A280370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(n)=3*2^(n-1) if n is even and n>1,
 *            a(n)=2^(n-2)-1 if n is odd and n>1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* compute 2^n */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    if ((n & 1) == 0) {          /* even n > 1 */
        seq_t half = pow2 / (seq_t)2;
        return half * (seq_t)3;
    } else {                      /* odd n > 1 */
        seq_t quarter = pow2 / (seq_t)4;
        return quarter - (seq_t)1;
    }
}
