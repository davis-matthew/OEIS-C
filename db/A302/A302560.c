/*
 * A302560  Partial sums of icosahedral numbers (A006564).
 * https://oeis.org/A302560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2 - 3*n + 10*n^2 + 15*n^3)/24
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 221, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of icosahedral numbers (A302560).  a(1) is the first term. */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n == 0 simply return 0
       to keep the function well‑defined. */
    if (n == 0) return 0;

    /* Work in seq_t to avoid truncation when n is large. */
    seq_t N = (seq_t)n;                       /* cast index to seq_t */

    /* Compute the polynomial 2 - 3*n + 10*n^2 + 15*n^3. */
    seq_t term3   = (seq_t)3   * N;               /* 3*n          */
    seq_t term10  = (seq_t)10  * N * N;           /* 10*n^2       */
    seq_t term15  = (seq_t)15  * N * N * N;       /* 15*n^3       */
    seq_t inner   = ((seq_t)2) - term3 + term10 + term15;

    /* Multiply by n and divide by 24.  The division is exact. */
    seq_t num = N * inner;
    return num / (seq_t)24;
}
