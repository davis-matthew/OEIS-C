/*
 * A247248  a(n) is the least k such that n divides 2^k + k.
 * https://oeis.org/A247248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = least k such that n | 2^k + k */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined but safe */
    if (n == 1) return (seq_t)1;  /* 2^1 + 1 = 3 divisible by 1 */

    idx_t two   = 1 % n;          /* holds 2^k (mod n) */
    idx_t k_mod = 1 % n;          /* holds k (mod n) */

    for (idx_t k = 1;; ++k)
    {
        two = ((unsigned __int128)two * 2) % n;   /* update 2^k (mod n) */
        if ((two + k_mod) % n == 0)
            return (seq_t)k;                      /* found minimal k */

        /* advance k mod n without overflow */
        k_mod++;
        if (k_mod >= n)
            k_mod -= n;
    }
}
