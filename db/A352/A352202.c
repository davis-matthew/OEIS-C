/*
 * A352202  a(n) = binary weight of A115510(n).
 * https://oeis.org/A352202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binary weight of A115510(n) -- a(n) = popcount(n) except
 *            -- a(n) = 2 when n is a power of two (n>1), -- a(n) = 1 when n =
 *            2^k + 1 (k ≥ 1), -- a(1) = 1
 * Range:     u8 n <= 116, u16 n <= 116, u32 n <= 116, u64 n <= 116,
 *            u128 n <= 116, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special offset */
    if (n == 1) return (seq_t)1;

    /* popcount of n */
    seq_t pc = 0;
    idx_t t = n;
    while (t) {
        pc++;
        t &= t - 1;          /* drop lowest set bit */
    }

    /* power of two (except 1) */
    if (n > 1 && (n & (n - 1)) == 0)
        return (seq_t)2;

    /* n-1 is a power of two (k ≥ 1) */
    if ((n - 1) > 0 && ((n - 1) & (n - 2)) == 0)
        return (seq_t)1;

    return pc;
}
