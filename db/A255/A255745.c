/*
 * A255745  a(1) = 1; for n > 1, a(n) = 11*10^{A000120(n-1)-1}.
 * https://oeis.org/A255745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, for n = 1; a(n) = 11 * 10^(popcount(n-1) - 1), for n >
 *            1
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* popcount(n-1) is the number of 1‑bits in the binary representation of n-1 */

static unsigned popcount64(idx_t v)
{
    unsigned c = 0;
    while (v) {
        v &= v - 1;   /* clear the least significant bit set */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }

    idx_t m = n - 1;                 /* n > 1 so m >= 1 */
    unsigned pc = popcount64(m);     /* popcount of n-1 */
    unsigned exp = pc - 1;           /* exponent for 10, always >= 0 */

    seq_t val = (seq_t)11;
    while (exp--) {
        val = val * (seq_t)10;
    }
    return val;
}
