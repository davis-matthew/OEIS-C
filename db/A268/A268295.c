/*
 * A268295  Terms at square positions in Pascal's triangle when in flattened
 *          form.
 * https://oeis.org/A268295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 52, u128 n <= 98,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = C(r, k) where r = floor((sqrt(8*n²+1)-1)/2),
 *             k = n² - r(r+1)/2.
 *  This is the term at index n² in the flattened Pascal triangle.
 */

#include <math.h>          /* for sqrt */
#include "../utils/binom.h"

static inline idx_t isqrt_u128(uint64_t n)
{
    /* integer square root of n, works for n upto ~2^66 (our usage) */
    double d = sqrt((double)n);
    idx_t x = (idx_t)d;
    /* adjust upwards */
    while ((unsigned __int128)(x + 1) * (x + 1) <= n) ++x;
    /* adjust downwards */
    while ((unsigned __int128)x * x > n) --x;
    return x;
}

seq_t
a(idx_t n)
{
    /* Position in the flattened triangle */
    idx_t i = n * n;              /* n² : the required index */

    /* Compute r = floor((sqrt(8*i+1)-1)/2) using integer sqrt */
    idx_t y = isqrt_u128((uint64_t)8 * i + 1);
    idx_t r = (y - 1) / 2;

    /* k = i - T_r where T_r = r*(r+1)/2 */
    seq_t tri = (seq_t)r * ((seq_t)r + 1) / 2;
    idx_t k = i - (idx_t)tri;

    return binom(r, k);
}
