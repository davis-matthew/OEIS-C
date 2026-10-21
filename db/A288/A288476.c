/*
 * A288476  a(n) = a(n-1) + 4*a(n-2) - 2*a(n-3), where a(0) = 2, a(1) = 4,
 *          a(2) = 8.
 * https://oeis.org/A288476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 2*a(n-3), a(0)=2, a(1)=4, a(2)=8
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 51, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms explicitly. */
    if (n == 0u) return (seq_t)2;
    if (n == 1u) return (seq_t)4;
    if (n == 2u) return (seq_t)8;

    /* Use three sliding variables to evaluate the recurrence. */
    seq_t a0 = (seq_t)2;   /* a(n-3) */
    seq_t a1 = (seq_t)4;   /* a(n-2) */
    seq_t a2 = (seq_t)8;   /* a(n-1) */
    seq_t a3;              /* a(n) */

    /* Compute from 3 up to n. */
    for (idx_t i = 3; i <= n; ++i) {
        a3 = a2 + (seq_t)4 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
