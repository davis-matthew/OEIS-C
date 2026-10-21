/*
 * A289877  Number of maximal cliques in the n-triangular honeycomb queen
 *          graph.
 * https://oeis.org/A289877
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(22 - n + 2*n^2) - 95 - (-1)^n)/16
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the offset of the sequence is not defined.
       The function returns 0 for such inputs. */
    if (n < 2)
        return 0;

    seq_t m = (seq_t)n;

    /* Compute the numerator:
         4*n^3 - 2*n^2 + 44*n - 95  */
    seq_t numerator = 4 * m * m * m - 2 * m * m + 44 * m - 95;

    /* Correct for the (-1)^n term:
         even n  → subtract 1
         odd  n  → add 1    */
    if ((m & 1) == 0)
        numerator -= (seq_t)1;
    else
        numerator += (seq_t)1;

    /* Final division by 16 (exact for all integer n >= 2) */
    return numerator / (seq_t)16;
}
