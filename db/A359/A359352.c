/*
 * A359352  a(n) = A026430(1 + A026430(n)).
 * https://oeis.org/A359352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = S(1 + S(n)), where S = A026430, S(2k) = 3k, S(2k+1) = 3k
 *            + 1 + (parity of popcount(k))
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A026430_s(seq_t m)
{
    seq_t k = m >> 1;
    seq_t r = (seq_t)3 * k;
    if ((m & 1) != 0)
    {
        int p = 0;
        seq_t t = k;
        while (t > 0)
        {
            if ((t & 1) != 0) p ^= 1;
            t >>= 1;
        }
        r += 1 + p;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t s = A026430_s((seq_t)n);
    return A026430_s(s + 1);
}
