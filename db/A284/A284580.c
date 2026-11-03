/*
 * A284580  Carryless base-2 product (A048720) of lengths of runs of 1-bits in
 *          binary representation of n.
 * https://oeis.org/A284580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = carryless base-2 product of lengths of runs of 1-bits in
 *            binary representation of n
 * Range:     u8 n <= 10922, u16 n <= 10922, u32 n <= 10922, u64 n <= 10922,
 *            u128 n <= 10922, big any n
 * Validated: standard, pass, 137 terms through n = 9573 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t cl_mult(seq_t a, seq_t b)
{
    seq_t res = 0;
    while (b)
    {
        if (b & 1)
            res ^= a;
        a <<= 1;
        b >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t x = n;
    while (x)
    {
        while ((x & 1) == 0)
            x >>= 1;
        idx_t len = 0;
        while (x & 1)
        {
            ++len;
            x >>= 1;
        }
        prod = cl_mult(prod, (seq_t)len);
    }
    return prod;
}
