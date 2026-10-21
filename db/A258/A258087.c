/*
 * A258087  Start with all terms set to 0. Then add n to the next n+2 terms
 *          for n=0,1,2,... .
 * https://oeis.org/A258087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n^2+2*n-11+(2*n-5)*(-1)^n)/16+0^n
 * Range:     u8 n <= 25, u16 n <= 417, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses only integer arithmetic, handling parity to avoid
   negative intermediate values. */

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the term is 0 (0^n adds 1 only for n=0,
       which is cancelled by the -1 from the division). */
    if (n <= 1)
        return 0;

    if (n & 1) {                         /* n is odd */
        seq_t prev = (seq_t)(n - 1);     /* n-1 */
        seq_t next = (seq_t)(n + 1);     /* n+1 */
        /* a(n) = 6*(n-1)*(n+1)/16 */
        return ((seq_t)6 * prev * next) / 16;
    } else {                              /* n is even */
        /* a(n) = (6*n^2 + 4*n - 16)/16 */
        return (((seq_t)6 * n * n) + ((seq_t)4 * n) - (seq_t)16) / 16;
    }
}
