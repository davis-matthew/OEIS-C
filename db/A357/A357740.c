/*
 * A357740  Number of non-equivalent ways under symmetry in one axis that 2
 *          non-attacking kings of different colors can be placed on an n X n
 *          board.
 * https://oeis.org/A357740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n-1)*((n+3)*n - 2 + (n mod 2))/2, n ≥ 1
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    /* Compute (n-2)*(n-1)/2  -- the product of two consecutive numbers
       is always even, so division by 2 is exact and prevents overflow. */
    seq_t termA = (seq_t)(n - 2) * (seq_t)(n - 1);
    termA = termA / 2;

    /* Compute ((n+3)*n - 2 + (n mod 2)) */
    seq_t termB = ((seq_t)(n + 3) * (seq_t)n) - 2 + (seq_t)(n & 1ULL);

    /* Final result */
    return termA * termB;
}
