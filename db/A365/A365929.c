/*
 * A365929  Number of intersections formed within a triangle by placing n
 *          points "in general position" on each of the three sides and
 *          connecting each point to each of the points on the other two sides
 *          using straight lines.
 * https://oeis.org/A365929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 209, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A365929:
   a(n) = (3/4)*n^2*(n-1)*(3*n-1)   (with a(0)=0, a(1)=0) */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    if ((n & 1) == 0) {          /* even n : use n = 2*k  */
        seq_t k = (seq_t)(n / 2);
        seq_t term = k * k;                      /* k^2          */
        term *= ((seq_t)n - 1);                  /* * (n-1)      */
        term *= ((seq_t)3 * (seq_t)n - 1);       /* * (3n-1)     */
        term *= 3;                               /* * 3          */
        return term;
    } else {                     /* odd n : use k = (n-1)/2  */
        seq_t k = ((seq_t)n - 1) / 2;
        seq_t term = (seq_t)n * (seq_t)n;         /* n^2          */
        term *= k;                               /* * k          */
        term *= ((seq_t)3 * k + 1);              /* * (3k+1) = (3n-1)/2   */
        term *= 3;                               /* * 3          */
        return term;
    }
}
