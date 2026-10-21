/*
 * A261892  Least multiple m of n such that n and m have no common one bit in
 *          their binary representations.
 * https://oeis.org/A261892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 286, u32 n <= 9364, u64 n <= 9364,
 *            u128 n <= 9364, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate k until (n*k) & n == 0 to find the least multiple m of n
   that shares no '1' bits with n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* offset starts at 1, safeguard */

    seq_t sn = (seq_t)n;                  /* cast index to seq_t for multiplication */
    for (idx_t k = 1; ; ++k)
    {
        seq_t prod = sn * (seq_t)k;       /* m = n * k */
        if ( (prod & sn) == (seq_t)0 )    /* no common one bits */
            return prod;
    }
}
