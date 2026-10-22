/*
 * A342300  Least nonnegative number greater than the previous number which is
 *          simultaneously an n-gonal and (n+1)-gonal number.
 * https://oeis.org/A342300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 32*n^5 - 112*n^4 + 70*n^3 + 93*n^2 - 57*n - 35 for n > 3
 *            a(0) = 0, a(1) = 1, a(2) = 3, a(3) = 36
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)36;

    /* Compute polynomial in seq_t to avoid overflow for large n when seq_t is 128‑bit */
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;          /* n^2 */
    seq_t n3 = n2 * N;         /* n^3 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t n5 = n4 * N;         /* n^5 */

    seq_t term = (seq_t)32 * n5
                - (seq_t)112 * n4
                + (seq_t)70  * n3
                + (seq_t)93  * n2
                - (seq_t)57  * N
                - (seq_t)35;
    return term;
}
