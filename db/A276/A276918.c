/*
 * A276918  a(2n) = A060867(n+1), a(2n+1) = A092440(n+1).
 * https://oeis.org/A276918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-3) + 4*a(n-4) (n>3) Initial values:
 *            a(0)=1, a(1)=5, a(2)=9, a(3)=25 This implementation computes the
 *            term iteratively, using only the allowed arithmetic operators
 *            and types
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)25;

    /* Initialise the four most recent terms */
    seq_t prev4 = (seq_t)1;   /* a(n-4) */
    seq_t prev3 = (seq_t)5;   /* a(n-3) */
    seq_t prev2 = (seq_t)9;   /* a(n-2) */
    seq_t prev1 = (seq_t)25;  /* a(n-1) */

    seq_t current = 0;        /* placeholder for a(n) */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 3*a(i-1) - 6*a(i-3) + 4*a(i-4) */
        seq_t t1 = (seq_t)3 * prev1;
        seq_t t2 = (seq_t)6 * prev3;
        seq_t t3 = (seq_t)4 * prev4;
        current = t1 - t2 + t3;

        /* Shift the window of recent terms */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}
