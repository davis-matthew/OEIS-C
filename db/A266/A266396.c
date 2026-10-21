/*
 * A266396  Number of orbits of Aut(Z^7) as function of the infinity norm n of
 *          the representative lattice point of the orbit, when the
 *          cardinality of the orbit is equal to 80640.
 * https://oeis.org/A266396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4+30*n^3-205*n^2+390*n-216)/24
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 388, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1   (a(1) is the first term) */
seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;        /* cast index to seq_t */
    seq_t n2  = N * N;           /* n^2 */
    seq_t n3  = n2 * N;          /* n^3 */
    seq_t n4  = n2 * n2;         /* n^4 */

    seq_t num = n4;                                   /* n^4 */
    num += (seq_t)30 * n3;                            /* +30*n^3 */
    num -= (seq_t)205 * n2;                           /* -205*n^2 */
    num += (seq_t)390 * N;                            /* +390*n */
    num -= (seq_t)216;                                /* -216 */

    return num / (seq_t)24;                           /* divide by 24 */
}
