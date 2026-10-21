/*
 * A251130  Number of (n+1) X (1+1) 0..2 arrays with no 2 X 2 subblock having
 *          the sum of its diagonal elements greater than the maximum of its
 *          antidiagonal elements.
 * https://oeis.org/A251130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) = 5*a(n-1) - 8*a(n-2) + 2*a(n-3) + 7*a(n-4) - 7*a(n-5) + 2*a(n-6)
   Initial terms (n=1..6): 40, 133, 369, 919, 2129, 4699 */

seq_t a(idx_t n)
{
    /* Base cases for the first six values */
    if (n == 1) return (seq_t)40;
    if (n == 2) return (seq_t)133;
    if (n == 3) return (seq_t)369;
    if (n == 4) return (seq_t)919;
    if (n == 5) return (seq_t)2129;
    if (n == 6) return (seq_t)4699;

    /* Sliding window of the last six terms:
       a0 = a(n-6), a1 = a(n-5), a2 = a(n-4), a3 = a(n-3),
       a4 = a(n-2), a5 = a(n-1) */
    seq_t a0 = (seq_t)40;
    seq_t a1 = (seq_t)133;
    seq_t a2 = (seq_t)369;
    seq_t a3 = (seq_t)919;
    seq_t a4 = (seq_t)2129;
    seq_t a5 = (seq_t)4699;

    for (idx_t i = 7; i <= n; ++i) {
        /* Compute the positive and negative parts separately to avoid
           intermediate underflow in unsigned arithmetic.  For all
           n ≥ 7 the positive part is strictly larger. */
        seq_t pos = 5 * a5 + 2 * a3 + 7 * a2 + 2 * a0;
        seq_t neg = 8 * a4 + 7 * a1;
        seq_t cur = pos - neg;   /* guaranteed non‑negative */

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = cur;
    }

    return a5;   /* a(n) */
}
