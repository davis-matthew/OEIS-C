/*
 * A339210  a(n) = Sum_{k=0..(n+1)/2} A010815(n*k).
 * https://oeis.org/A339210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor((n+1)/2)} A010815(n*k)
 * Range:     i8 n <= 76, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A010815(x) = (-1)^m if x = m(3m±1)/2 for some integer m,
   otherwise 0. */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for non‑negative x. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62;          /* highest power of 4 <= 2^64 */
    while (bit)
    {
        if (x >= r + bit)
        {
            x -= r + bit;
            r = (r >> 1) + bit;
        }
        else
        {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* A010815(x): sign of a generalized pentagonal number. */
static inline seq_t pentagonal_sign(idx_t x)
{
    if (x == 0)                 /* m = 0 gives value 1 */
        return 1;

    /* D = 1 + 24*x, fits in 128 bits for all intended n. */
    unsigned __int128 D = (unsigned __int128)24 * (unsigned __int128)x + 1;
    idx_t s = isqrt_u64((idx_t)D);
    if ((unsigned __int128)s * s != D)
        return 0;

    /* Check for m such that (s+1)/6 or (s-1)/6 is integer. */
    if ((s + 1) % 6 == 0)
    {
        seq_t m = ((seq_t)(s + 1)) / 6;
        return (m & 1) ? (seq_t)(-1) : (seq_t)1;
    }
    if ((s + 5) % 6 == 0)       /* (s-1) divisible by 6 */
    {
        seq_t m = ((seq_t)(s - 1)) / 6;
        return (m & 1) ? (seq_t)(-1) : (seq_t)1;
    }
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0U)
        return 0;                /* undefined below offset, but safe */

    idx_t limit = (n + 1) / 2;
    seq_t sum = 1;                /* k = 0 contributes 1 */

    for (idx_t k = 1; k <= limit; ++k)
    {
        idx_t prod = n * k;
        sum += pentagonal_sign(prod);
    }

    return sum;
}
