/*
 * A355141  a(0)=1, a(1)=a(2)=0; for n > 2, a(n) = a(n-1) + s if n is odd,
 *          a(n-1) - s if n is even, where s = a(n-1) + a(n-2) + a(n-3).
 * https://oeis.org/A355141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + (2*(n mod 2)-1)*(a(n-3)+a(n-2)+a(n-1)), a(0) =
 *            1, a(1) = 0, a(2) = 0
 * Range:     i8 n <= 52, i16 n <= 110, i32 n <= 228, i64 n <= 462,
 *            i128 n <= 922, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes theOEIS sequence A355141   (recurrence form)   */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)0;

    /* Sliding window: a0 = a(n-3), a1 = a(n-2), a2 = a(n-1)   */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)0;   /* a(1) */
    seq_t a2 = (seq_t)0;   /* a(2) */

    seq_t ai;               /* a(i)   */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t s = a2 + a1 + a0;           /* sum of last three terms   */
        if (i & 1)              /* i is odd  */
            ai = a2 + s;        /* a(i) = a(i-1) + s         */
        else
            ai = a2 - s;        /* a(i) = a(i-1) - s         */

        /* shift the window for next iteration     */
        a0 = a1;
        a1 = a2;
        a2 = ai;
    }

    return a2;   /* a(n) is now in a2 */
}
