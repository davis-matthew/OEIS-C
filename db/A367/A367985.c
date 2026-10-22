/*
 * A367985  Number of 4-cycles in the n-cycle complement and (n+1)-wheel
 *          complement graph.
 * https://oeis.org/A367985
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 5) * n * (n^2 - 9*n + 22) / 8 for n >= 5 Initial
 *            values: a(3) = 0, a(4) = 0, a(5) = 0 (For n < 5 the sequence is
 *            0.)
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)  /* for n = 3,4,5 the value is 0 */
        return 0;

    seq_t sn  = (seq_t)n;                /* cast index to seq_t */
    seq_t t1  = sn - 5;                  /* (n - 5)            */
    seq_t t2  = sn;                      /* n                  */
    seq_t t3  = sn * sn - 9 * sn + 22;   /* n^2 - 9n + 22      */

    seq_t num = t1 * t2;                 /* (n - 5) * n        */
    num = num * t3;                      /* (n - 5) * n * (n^2 - 9n + 22) */

    return num / 8;                      /* divide by 8         */
}
