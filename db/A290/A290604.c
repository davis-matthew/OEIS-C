/*
 * A290604  a(0) = 2, a(1) = 2; for n > 1, a(n) = a(n-1) + 2*a(n-2) + 3.
 * https://oeis.org/A290604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 2, a(1) = 2; for n > 1, a(n) = a(n-1) + 2*a(n-2) + 3
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)2;

    seq_t pprev = (seq_t)2;   /* a(0) */
    seq_t prev  = (seq_t)2;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev + (seq_t)2 * pprev + (seq_t)3;
        pprev = prev;
        prev  = cur;
    }
    return prev;
}
