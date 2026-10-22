/*
 * A362179  Main diagonal of the square array A058395.
 * https://oeis.org/A362179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 12*a(n-2) + 8*a(n-3) for n > 5 (initial
 *            values: a(0)=1, a(1)=1, a(2)=4, a(3)=10, a(4)=25, a(5)=61)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the known initial terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)25;
    if (n == 5) return (seq_t)61;

    /* recurrence for n >= 6 */
    seq_t a3 = (seq_t)10;   /* a(n-3) for n=6 */
    seq_t a4 = (seq_t)25;   /* a(n-2) for n=6 */
    seq_t a5 = (seq_t)61;   /* a(n-1) for n=6 */
    seq_t an;

    /* iterate n-5 times to reach a(n) */
    for (idx_t i = 0; i < n - 5; ++i) {
        an = (seq_t)6 * a5 - (seq_t)12 * a4 + (seq_t)8 * a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return a5;
}
