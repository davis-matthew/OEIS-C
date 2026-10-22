/*
 * A333296  Largest number of non-congruent integer-sided bricks that can be
 *          assembled into an n X n X n cube.
 * https://oeis.org/A333296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 1, a(2) = 1 a(n) = n*(n+1)/2 for 3 ≤ n ≤ 7 a(n) =
 *            n*(n-1)/2 + 7 for n ≥ 8
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 1;

    seq_t nn = (seq_t)n;          /* cast index to value type once */

    if (n <= 7)
        return nn * (nn + 1) / 2; /* triangular number */

    return nn * (nn - 1) / 2 + 7; /* triangular of (n‑1) plus 7 */
}
