/*
 * A390825  a(n) = Sum_{k=0..n} binomial(n+2*k,n-k) * Fibonacci(k+1).
 * https://oeis.org/A390825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+2k, n-k) * Fibonacci(k+1)
 *            Recurrence: a(n) = 7*a(n-1) - 17*a(n-2) + 23*a(n-3) - 16*a(n-4)
 *            + 6*a(n-5) - a(n-6) Initial values: a(0)=1, a(1)=2, a(2)=7,
 *            a(3)=28, a(4)=112, a(5)=442 This implementation uses the
 *            recurrence; it is O(n) per call and requires no external st...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 7;
    if (n == 3) return 28;
    if (n == 4) return 112;
    if (n == 5) return 442;

    /* Recurrence for n >= 6 */
    seq_t a0 = 1;   /* a(n-6) */
    seq_t a1 = 2;   /* a(n-5) */
    seq_t a2 = 7;   /* a(n-4) */
    seq_t a3 = 28;  /* a(n-3) */
    seq_t a4 = 112; /* a(n-2) */
    seq_t a5 = 442; /* a(n-1) */
    seq_t cur;

    for (idx_t i = 6; i <= n; ++i) {
        cur = 7 * a5
              - 17 * a4
              + 23 * a3
              - 16 * a2
              + 6 * a1
              - a0;

        /* Shift the last six values */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = cur;
    }

    return a5;
}
