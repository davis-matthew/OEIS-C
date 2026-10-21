/*
 * A247904  Start with a single pentagon; at n-th generation add a pentagon at
 *          each expandable vertex (this is the "vertex to side" version);
 *          a(n) is the sum of all label values at n-th generation. (See
 *          comment for construction rules.)
 * https://oeis.org/A247904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*2^n - (5*n + 9)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow2_u64(idx_t n)
{
    seq_t r = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        r <<= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t p   = pow2_u64(n);
    seq_t term = (seq_t)10 * p - (((seq_t)5 * (seq_t)n) + (seq_t)9);
    return term;
}
