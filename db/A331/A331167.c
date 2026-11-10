/*
 * A331167  a(n) = min(n, A193231(n)), where A193231(n) is blue code of n.
 * https://oeis.org/A331167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(n, A193231(n)), where A193231(n) = XOR over set bits
 *            e of n of (x+1)^e as GF(2) polynomial
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 112 terms through n = 9814 (2026-11-08)
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
    seq_t b = A193231_a(n);
    seq_t m = (seq_t)n;
    return (b < m) ? b : m;
}
