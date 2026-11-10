/*
 * A280507  a(n) = n XOR A193231(n).
 * https://oeis.org/A280507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n XOR A193231(n), where A193231(n) = XOR over set bits e
 *            of n of the polynomial (x+1)^e over GF(2) (binary coded)
 * Range:     u8 n <= 511, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 114 terms through n = 9657 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A193231_a(idx_t n)
{
    seq_t res  = 0;
    seq_t mask = 1;
    idx_t t = n;

    while (t) {
        if (t & 1)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        t >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    return (seq_t)n ^ A193231_a(n);
}
