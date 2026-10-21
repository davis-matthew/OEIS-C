/*
 * A276417  a(n) = least positive k such that (2*n + 1) - 2^k is prime, or 0
 *          if no such k exists.
 * https://oeis.org/A276417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive k such that (2*n + 1) - 2^k is prime, or 0
 *            if none
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t m   = (seq_t)n * 2 + 1;   /* compute 2*n + 1 */
    seq_t pow2 = 2;                 /* 2^1 */
    seq_t k    = 1;                 /* current exponent */

    while (pow2 <= m)
    {
        seq_t candidate = m - pow2;
        if (is_prime_big(candidate))
            return k;
        pow2 <<= 1;                 /* multiply by 2 to get next power */
        ++k;
    }
    return 0;                       /* no suitable k found */
}
