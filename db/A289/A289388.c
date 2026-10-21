/*
 * A289388  a(n) = Sum_{k>=0} (-1)^k*binomial(n,5*k+3).
 * https://oeis.org/A289388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 11, i16 n <= 17, i32 n <= 34, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289388
 * a(n) = Sum_{k>=0} (-1)^k * binomial(n, 5k+3)
 * Recurrence (for n ≥ 5):
 *   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4)
 * Initial values:
 *   a(0) = 0, a(1) = 0, a(2) = 0, a(3) = 1, a(4) = 4 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 0;
    if (n == 3) return 1;
    if (n == 4) return 4;

    /* sliding window: a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = 0; /* a(0) */
    seq_t a1 = 0; /* a(1) */
    seq_t a2 = 0; /* a(2) */
    seq_t a3 = 1; /* a(3) */
    seq_t a4 = 4; /* a(4) */
    seq_t an = 0;

    /* we will compute from n = 5 up to the requested n */
    for (idx_t i = 5; i <= n; ++i) {
        an = (seq_t)5 * a4
           - (seq_t)10 * a3
           + (seq_t)10 * a2
           - (seq_t)5  * a1;

        /* shift the window: drop the oldest, shift others, insert new */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
