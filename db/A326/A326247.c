/*
 * A326247  Number of labeled n-vertex 2-edge multigraphs that are neither
 *          crossing nor nesting.
 * https://oeis.org/A326247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n * (12 - 19*n + 6*n^2 + n^3)) / 12
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0)=0, a(1)=0, a(2)=1, …  (OEIS A326247)  */
seq_t a(idx_t n)
{
    const seq_t nn   = (seq_t)n;      /* cast index to seq_t */
    const seq_t n2   = nn * nn;        /* n^2  */
    const seq_t n3   = n2 * nn;        /* n^3  */
    const seq_t n4   = n3 * nn;        /* n^4  */

    /* Compute numerator = 12*n + 6*n^3 + n^4 - 19*n^2 */
    seq_t numerator = 12 * nn;
    numerator += 6 * n3;
    numerator += n4;
    numerator -= 19 * n2;

    return numerator / 12; /* exact division */
}
