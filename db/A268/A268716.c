/*
 * A268716  a(n) = 2*A006068(n); main diagonal of A268714.
 * https://oeis.org/A268716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * A006068(n) where A006068 is defined by A006068(0) = 0
 *            and A006068(n) = (2 * 2^⌊log2 n⌋ - 1) - A006068(n - 2^⌊log2 n⌋)
 *            for n > 0 This recursively computes a permutation of the
 *            integers and yields a(0) = 0, a(1) = 2, a(2) = 6, etc
 * Range:     u8 n <= 127, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 90 terms through n = 7023 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t _a006068(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find largest power of two ≤ n */
    idx_t p = 1;
    while ((p << 1) <= n)
        p <<= 1;

    idx_t r = n - p;                       /* n = p + r, 0 ≤ r < p */
    seq_t high = ((seq_t)p << 1) - 1;      /* (2 * p) - 1   */
    seq_t low  = _a006068(r);             /* recursive call */

    return high - low;
}

seq_t a(idx_t n)
{
    /* a(n) = 2 * A006068(n) */
    return (seq_t)2 * _a006068(n);
}
