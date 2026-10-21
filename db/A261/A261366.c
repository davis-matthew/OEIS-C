/*
 * A261366  a(n) = number of even terms in row n of triangle A261363.
 * https://oeis.org/A261366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+1 - 2^{popcount(n-1)}
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)           /* a(0) = 0 by definition */
        return 0;

    /* popcount of n-1: number of 1‑bits */
    idx_t m = n - 1;
    unsigned pc = 0;
    while (m) {
        m &= m - 1;
        pc++;
    }

    seq_t pow2 = ((seq_t)1) << pc;      /* 2^{popcount} */
    return ((seq_t)n) + 1 - pow2;       /* n+1 - 2^{popcount} */
}
