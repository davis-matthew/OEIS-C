/*
 * A306150  Row sums of A306015.
 * https://oeis.org/A306150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=2, a(2)=4, a(n)=n*a(n-1)+a(n-2)-(n-2)*a(n-3) for
 *            n≥3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)2;   /* a(n-2) */
    seq_t a2 = (seq_t)4;   /* a(n-1) */
    seq_t next;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(n) = n*a(n-1) + a(n-2) - (n-2)*a(n-3) */
        next = (seq_t)i * a2 + a1 - (seq_t)(i - 2) * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
