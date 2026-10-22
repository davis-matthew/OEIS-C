/*
 * A341509  a(n) = 2^j if n is of the form 2^i - 2^j with i > j, and 0
 *          otherwise.
 * https://oeis.org/A341509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 9772, u32 n <= 9772, u64 n <= 9772,
 *            u128 n <= 9772, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A341509
    a(n) = 2^j  if there exist integers i>j>=0 with n = 2^i – 2^j,
              otherwise a(n) = 0.
    The values are returned as a seq_t and computed directly from the
    binary representation of n.  The algorithm runs in O(#trailing zeros)
    time, which is at most O(log n). */
#include <stdint.h>

static idx_t count_trailing_zeros(idx_t x)
{
    idx_t c = 0;
    /* fast path: use compiler builtin if available */
#if defined(__GNUC__) || defined(__clang__)
    if (x)
        return __builtin_ctzll(x);
#endif
    /* generic fallback – cheap for the small values typical of this test */
    while ((x & 1) == 0) {
        x >>= 1;
        c++;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Separate the largest power of two dividing n.  j = v2(n). */
    idx_t j = count_trailing_zeros(n);

    /* The odd part of n.  If n is 0, j would be all bits; this case
       is handled above. */
    idx_t m = n >> j;

    /* If m+1 is a power of two then n = 2^j * (2^k – 1) with k>0,
       i.e. n = 2^(j+k) – 2^j exactly as required.  */
    idx_t mp1 = m + 1ULL;
    /* Guard against overflow when m is UINT64_MAX (which would only
       happen for n = 2^64-1, an extreme case).  In that impossible
       situation we return 0. */
    if (m == UINT64_MAX)
        return 0;

    if ((mp1 & m) == 0) {
        /* Return 2^j as a seq_t value.  */
        return ((seq_t)1) << j;
    }

    return 0;
}
