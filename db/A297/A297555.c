/*
 * A297555  a(n) = a(n-1) + 16*a(n-3) - 16*a(n-4), where a(0) = 1, a(1) = 4,
 *          a(2) = 8, a(3) = 12, a(4) = 76.
 * https://oeis.org/A297555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 16*a(n-3) - 16*a(n-4); Initial values: a(0)=1,
 *            a(1)=4, a(2)=8, a(3)=12, a(4)=76
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five known terms */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)4;
    if (n == (idx_t)2) return (seq_t)8;
    if (n == (idx_t)3) return (seq_t)12;
    if (n == (idx_t)4) return (seq_t)76;

    /* Initialise the variables for the recurrence.
       a4 holds a(n-1), a2 holds a(n-3), a1 holds a(n-4). */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)4;
    seq_t a2 = (seq_t)8;
    seq_t a3 = (seq_t)12;
    seq_t a4 = (seq_t)76;

    for (idx_t i = (idx_t)5; i <= n; ++i) {
        seq_t new_val = a4 + (seq_t)16 * a2 - (seq_t)16 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = new_val;
    }
    return a4;
}
