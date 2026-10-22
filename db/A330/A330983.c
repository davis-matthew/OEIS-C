/*
 * A330983  Alternatively add and multiply pairs of the nonnegative integers.
 * https://oeis.org/A330983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n - 3, if n is odd a(n) = (2*n - 2)*(2*n - 1), if n is
 *            even Notes: a(1)=1, a(2)=6, a(3)=9, a(4)=42, ... The function
 *            handles n>=1 in any call order
 * Range:     u8 n <= 9, u16 n <= 129, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned 64‑bit. Convert to seq_t inside the expression
       to ensure the whole calculation uses the correct integer type. */
    if (n & 1U) {
        /* Odd index: 4*n - 3 */
        return (seq_t)4 * (seq_t)n - (seq_t)3;
    } else {
        /* Even index: (2*n - 2)*(2*n - 1) */
        seq_t t = (seq_t)2 * (seq_t)n;      /* 2*n */
        return (t - (seq_t)2) * (t - (seq_t)1);
    }
}
