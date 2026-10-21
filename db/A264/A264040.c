/*
 * A264040  Number of possible permutations of the n X n generalization of the
 *          sliding block 15-puzzle.
 * https://oeis.org/A264040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2)! / 2 for n > 1; a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1U;

    /* Compute (n^2)! */
    idx_t limit = n * n;
    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= limit; ++i)
    {
        res = res * (seq_t)i;
    }

    /* Divide by 2 */
    res = res / ((seq_t)2);
    return res;
}
