/*
 * A326700  Denominator of the average position of a 1 in the reversed binary
 *          expansion of n.
 * https://oeis.org/A326700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = popcount(n) / gcd(popcount(n), sum of positions of 1 bits
 *            in reversed binary expansion of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t _gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    idx_t tmp = n;
    idx_t w = 0;      /* popcount */
    idx_t sum = 0;    /* sum of 1-bit positions */
    idx_t pos = 1;

    while (tmp != 0)
    {
        if (tmp & 1)
        {
            ++w;
            sum += pos;
        }
        tmp >>= 1;
        ++pos;
    }

    idx_t g = _gcd_idx(sum, w);
    idx_t denom = w / g;
    return (seq_t)denom;
}
