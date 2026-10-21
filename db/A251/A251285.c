/*
 * A251285  Number of (n+1) X (1+1) 0..1 arrays with every 2 X 2 subblock
 *          having a single 1 or two 1s on the same edge or main diagonally.
 * https://oeis.org/A251285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - 3*a(n-3) + a(n-4) (initial values:
 *            a(1)=9, a(2)=22, a(3)=52, a(4)=125)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 23, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                          /* undefined for n<1, return 0 */
        return (seq_t)0;

    /* Handle the first four terms directly */
    if (n == 1) return (seq_t)9;
    if (n == 2) return (seq_t)22;
    if (n == 3) return (seq_t)52;
    if (n == 4) return (seq_t)125;

    /* Iterate the recurrence from 5 up to n */
    seq_t prev4 = (seq_t)9;   /* a(n-4) */
    seq_t prev3 = (seq_t)22;  /* a(n-3) */
    seq_t prev2 = (seq_t)52;  /* a(n-2) */
    seq_t prev1 = (seq_t)125; /* a(n-1) */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur = (seq_t)2 * prev1
            + (seq_t)2 * prev2
            - (seq_t)3 * prev3
            + prev4;

        /* Shift the window for the next step */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
