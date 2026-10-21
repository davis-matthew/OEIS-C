/*
 * A295686  a(n) = a(n-1) + a(n-3) + a(n-4), where a(0) = 2, a(1) = 1, a(2) =
 *          2, a(3) = 1.
 * https://oeis.org/A295686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), a(0) = 2, a(1) = 1, a(2) = 2,
 *            a(3) = 1
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    /* Sliding window holding a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)2; /* a(n-4) */
    seq_t a1 = (seq_t)1; /* a(n-3) */
    seq_t a2 = (seq_t)2; /* a(n-2) (unused after shift) */
    seq_t a3 = (seq_t)1; /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 + a1 + a0;  /* a(i) = a(i-1)+a(i-3)+a(i-4) */
        /* shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3; /* This is a(n) after the loop */
}
