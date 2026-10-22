/*
 * A396353  Number of unsatisfiable 3-SAT formulas with n variables and 3
 *          clauses in the multiset clause model.
 * https://oeis.org/A396353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(2*n^3 + 3*n^2 + 19*n - 18)/3
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t first */
    seq_t nn = (seq_t)n;

    /* Compute n^2 and n^3 */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* Inner polynomial */
    seq_t inner = (seq_t)2 * n3 + (seq_t)3 * n2 + (seq_t)19 * nn - (seq_t)18;

    /* (2*n) * inner */
    seq_t mult1 = (seq_t)2 * nn;
    seq_t numerator = mult1 * inner;

    /* Divide by 3 (exact division) */
    seq_t result = numerator / (seq_t)3;

    return result;
}
