/*
 * A292491  p-INVERT of the odd positive integers, where p(S) = 1 + S - 2 S^2.
 * https://oeis.org/A292491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 9*a(n-3) Initial values: a(0) = -1,
 *            a(1) = 0, a(2) = 8, a(3) = 16, a(4) = 24
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 20, i64 n <= 40, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pre‑defined terms for n = 0 … 4 */
    static const seq_t init[5] = { (seq_t)-1, (seq_t)0, (seq_t)8, (seq_t)16, (seq_t)24 };

    /* Return immediately for the first five indices */
    if (n < 5)
        return init[n];

    /* Sliding window of the last three computed values.
       We keep a2 = a(n-3), a3 = a(n-2), a4 = a(n-1). */
    seq_t a2 = init[2]; /* a(2) */
    seq_t a3 = init[3]; /* a(3) */
    seq_t a4 = init[4]; /* a(4) */
    seq_t an;           /* a(n) to be updated each iteration */

    /* Compute a(n) for n ≥ 5 */
    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)(3 * a4 - 3 * a3 + 9 * a2);
        /* Shift the window for the next step */
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return an;
}
