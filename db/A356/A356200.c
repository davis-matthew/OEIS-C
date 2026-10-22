/*
 * A356200  Number of edge covers in the n-gear graph.
 * https://oeis.org/A356200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 21*a(n-2) + 12*a(n-3) - 2*a(n-4) (n ≥ 5) *
 *            Initial values: a(1)=3, a(2)=25, a(3)=162, a(4)=969 * * This
 *            implementation evaluates the recurrence iteratively, using only
 *            * the types seq_t (the sequence value) and idx_t (unsigned
 *            integer * indexes). No dynamic memory allocation is...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the problematic cases explicitly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)25;
    if (n == 3) return (seq_t)162;
    if (n == 4) return (seq_t)969;

    /* when n < 1 (undefined) return 0;  callers normally use n ≥ 1 */
    if (n == 0) return (seq_t)0;

    /* keep the last four computed terms: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a1 = (seq_t)3,   /* a(n-4)   */
          a2 = (seq_t)25,  /* a(n-3)   */
          a3 = (seq_t)162, /* a(n-2)   */
          a4 = (seq_t)969; /* a(n-1)   */
    seq_t an;

    for (idx_t i = 5; i <= n; ++i) {
        an = 9 * a4 - 21 * a3 + 12 * a2 - 2 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return a4;
}
