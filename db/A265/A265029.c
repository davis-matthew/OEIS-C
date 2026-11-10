/*
 * A265029  First differences of A264619.
 * https://oeis.org/A265029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A264619(n) - A264619(n-1), where A264619(0)=1 and, for
 *            n>0, A264619(n) is the binary string n, then 1, then n
 *            bit-reversed, read as a binary number
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 32767, u64 n <= 32767,
 *            u128 n <= 32767, big any n
 * Validated: standard, pass, 75 terms through n = 9977 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A264619_a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    idx_t bits = 0;
    idx_t t = n;
    while (t)
    {
        t >>= 1;
        bits++;
    }
    seq_t rev = 0;
    for (idx_t i = 0; i < bits; ++i)
        rev = (rev << 1) | (seq_t)((n >> i) & 1);
    return ((seq_t)n << (bits + 1)) | ((seq_t)1 << bits) | rev;
}

seq_t a(idx_t n)
{
    return A264619_a(n) - A264619_a(n - 1);
}
