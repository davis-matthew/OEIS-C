/*
 * A241371  Number of 2 X n 0..2 arrays with no element equal to fewer
 *          vertical neighbors than horizontal neighbors, with new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A241371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2) - 4*a(n-3) initial values: a(1)=2,
 *            a(2)=7, a(3)=28, a(4)=121, a(5)=523
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly. */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)28;
    if (n == 4) return (seq_t)121;
    if (n == 5) return (seq_t)523;

    /* Maintain the last five values needed for the recurrence. */
    seq_t a1 = (seq_t)2;   /* a_{k-5} */
    seq_t a2 = (seq_t)7;   /* a_{k-4} */
    seq_t a3 = (seq_t)28;  /* a_{k-3} */
    seq_t a4 = (seq_t)121; /* a_{k-2} */
    seq_t a5 = (seq_t)523; /* a_{k-1} */

    for (idx_t k = 6; k <= n; ++k) {
        /* a(k) = 5*a(k-1) - 2*a(k-2) - 4*a(k-3) */
        seq_t next = 5 * a5 - 2 * a4 - 4 * a3;

        /* Shift the sliding window. */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
