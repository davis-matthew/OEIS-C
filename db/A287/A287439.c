/*
 * A287439  a(n) = 2*a(n-2) + 2*a(n-3) for n >= 3, where a(0) = 2, a(2) = 4,
 *          a(3) = 7.
 * https://oeis.org/A287439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 2*a(n-3) (n >= 3), a(0)=2, a(1)=4, a(2)=7
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 37, u64 n <= 76, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;

    /* Iteratively compute a(n) using the recurrence */
    seq_t a_k_minus_3 = (seq_t)2; /* a(0) */
    seq_t a_k_minus_2 = (seq_t)4; /* a(1) */
    seq_t a_k_minus_1 = (seq_t)7; /* a(2) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 2*a(i-2) + 2*a(i-3) */
        next = (seq_t)2 * a_k_minus_2 + (seq_t)2 * a_k_minus_3;

        /* Shift the window: a(k-3) <- a(k-2), a(k-2) <- a(k-1), a(k-1) <- next */
        a_k_minus_3 = a_k_minus_2;
        a_k_minus_2 = a_k_minus_1;
        a_k_minus_1 = next;
    }

    return a_k_minus_1; /* now holds a(n) */
}
