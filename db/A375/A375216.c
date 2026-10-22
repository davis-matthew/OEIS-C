/*
 * A375216  n is the a(n)-th nonnegative integer having its multiset of binary
 *          digits.
 * https://oeis.org/A375216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 3100, u16 n <= 9885, u32 n <= 9885, u64 n <= 9885,
 *            u128 n <= 9885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A375216:  a(n) is the rank (starting from 1) of n among all
    nonnegative integers having the same multiset of binary digits as n.
    The rank is computed by counting all numbers with the same bit length
    and the same popcount that are numerically smaller than n.  */

/*  Required headers */
#include <stdint.h>

/*  Compute popcount of a 64‑bit unsigned integer.  */
static unsigned popcount_uint64(uint64_t x)
{
    unsigned c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
}

/*  Compute floor(log2(x)) for x>0; returns 0 for x==0.  */
static unsigned floor_log2_uint64(uint64_t x)
{
    unsigned r = 0;
    while (x >> 1) { x >>= 1; ++r; }
    return r;
}

/*  Compute the binomial coefficient C(n,k) using a multiplicative
    algorithm that works for small n (n <= 64).  Handles k>n by
    returning 0.  */
static seq_t binomial_small(unsigned n, unsigned k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (unsigned i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    /* Special case: n = 0 */
    if (n == 0) return 1;

    /* Determine the binary length and popcount of n */
    unsigned len   = floor_log2_uint64(n) + 1U;       /* bit length */
    unsigned ones  = popcount_uint64(n);             /* number of ones */

    /* Remaining ones to place after fixing the leading 1 */
    unsigned remaining = (ones > 0) ? ones - 1 : 0;

    /* Rank starts at 1 (the number itself).  */
    seq_t rank = (seq_t)1;

    /* Iterate over lower bits from MSB‑1 downto 0 */
    for (int pos = (int)(len - 2); pos >= 0; --pos)
    {
        unsigned bit = (n >> (unsigned)pos) & 1U;
        if (bit)
        {
            /* If this bit were 0, count all ways to distribute
               'remaining' ones among the lower 'pos' positions. */
            rank += binomial_small((unsigned)pos, remaining);
            /* Consume this one from the remaining count */
            if (remaining) --remaining;
        }
        /* If bit == 0, nothing to do.  */
    }

    return rank;
}
