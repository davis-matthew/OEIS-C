/*
 * A370299  Number of chordless cycles in the complement of the n-Sierpinski
 *          gasket graph.
 * https://oeis.org/A370299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (72-17*3^n+9^n)/2 for n > 1 a(1) = 0, a(2) = 0 (OEIS
 *            A370299)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)2)
        return (seq_t)0;

    /* compute 3^n iteratively */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n; ++i)
        pow3 *= (seq_t)3;

    /* 9^n = (3^n)^2 */
    seq_t pow9 = pow3 * pow3;

    /* compute (72 - 17*3^n + 9^n) / 2 */
    seq_t result = (seq_t)72;              /* start with 72 */
    result -= (seq_t)17 * pow3;            /* subtract 17*3^n */
    result += pow9;                        /* add 9^n */
    result /= (seq_t)2;                    /* divide by 2 */

    return result;
}
