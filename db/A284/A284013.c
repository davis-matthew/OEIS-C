/*
 * A284013  a(n) = n - A002487(n).
 * https://oeis.org/A284013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A002487(n) where A002487(n) is Stern's diatomic
 *            series B(0)=0, B(1)=1, B(2k)=B(k), B(2k+1)=B(k)+B(k+1)
 * Range:     u8 n <= 263, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 102 terms through n = 7130 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t stern_b(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if ((n & 1) == 0)               /* even */
        return stern_b(n >> 1);
    seq_t left  = stern_b(n >> 1);
    seq_t right = stern_b((n >> 1) + 1);
    return left + right;
}

seq_t a(idx_t n)
{
    return (seq_t)n - stern_b(n);
}
