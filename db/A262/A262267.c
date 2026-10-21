/*
 * A262267  Number of (n+2) X (1+2) 0..1 arrays with each row and column
 *          divisible by 5, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A262267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - 2*a(n-4) a(1) = 2, a(2)
 *            = 4, a(3) = 7, a(4) = 13 (valid for n ≥ 5)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)13;

    /* Sliding window variables:
       a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1)
    */
    seq_t a1 = (seq_t)2;
    seq_t a2 = (seq_t)4;
    seq_t a3 = (seq_t)7;
    seq_t a4 = (seq_t)13;
    seq_t next;

    /* Iterate from 5 up to n, updating the window each time. */
    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)3 * a4
               - (seq_t)3 * a3
               + (seq_t)3 * a2
               - (seq_t)2 * a1;
        /* Shift the window for the next iteration. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;  /* After the loop a4 holds a(n). */
}
