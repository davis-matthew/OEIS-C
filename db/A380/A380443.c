/*
 * A380443  a(n) = numerator(b(n)), where b(n) = b(n/2)/2 for n even and 1 -
 *          b((n-1)/2)/2 for n odd with b(1) = 1.
 * https://oeis.org/A380443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, look
 * Formula:   a(n) = numerator(b(n)), where b(1) = 1 b(n) = b(n/2)/2 if n is
 *            even b(n) = 1 - b((n-1)/2)/2 if n is odd For odd n ≥ 3 : a(n) =
 *            2^k - a((n-1)/2) where k = ⌊log₂ n⌋
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses only integer arithmetic and recursion
    that is O(log n) in time and O(log n) in stack depth.  */

#include <stdint.h>
#include <stddef.h>

/* get floor(log2(n)) for n ≥ 1 */
static inline idx_t floor_log2_u64(idx_t n)
{
    /* __builtin_clzll is defined for n > 0 */
    return 63U - __builtin_clzll(n);
}

seq_t a(idx_t n)
{
    if (n == 1U)
        return (seq_t)1;

    if ((n & 1U) == 0U)          /* even case */
        return a(n >> 1U);

    /* odd case */
    idx_t k   = floor_log2_u64(n);
    seq_t pow = ((seq_t)1) << k;          /* 2^k */
    return pow - a((n - 1U) >> 1U);
}
