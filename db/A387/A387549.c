/*
 * A387549  Numbers k such that k^2 + 23 is twice a square.
 * https://oeis.org/A387549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4) with a(1)=3, a(2)=7, a(3)=25, a(4)=45
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first few values directly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)25;
    if (n == 4) return (seq_t)45;

    /* Keep the last four sequence values in a sliding window */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2 = (seq_t)7;   /* a(2) */
    seq_t a3 = (seq_t)25;  /* a(3) */
    seq_t a4 = (seq_t)45;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 6 * a3 - a1;  /* a(i) = 6*a(i-2) - a(i-4) */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;  /* a(n) */
}
