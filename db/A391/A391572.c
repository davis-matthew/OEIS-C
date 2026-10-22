/*
 * A391572  a(n) is the smallest odd k > 1 which divides A048720(n,k) [in
 *          ordinary arithmetic], where A048720 is carryless base-2
 *          multiplication.
 * https://oeis.org/A391572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest odd k > 1 such that k divides the carryless
 *            base‑2 product of n and k
 * Range:     u8 n <= 254, u16 n <= 9836, u32 n <= 9836, u64 n <= 9836,
 *            u128 n <= 9836, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; behaviour for n < 1 is undefined, return 0. */
    if (n < 1) return 0;

    /* Search odd k starting from 3. */
    for (uint64_t k = 3; ; k += 2) {
        unsigned __int128 prod = 0;
        unsigned __int128 tmp  = (unsigned __int128)n;
        unsigned __int128 kk   = (unsigned __int128)k;

        /* Compute carryless product of n and k using XOR of shifted n. */
        while (kk) {
            if (kk & 1) prod ^= tmp;
            tmp <<= 1;
            kk >>= 1;
        }

        if ((prod % k) == 0) {
            return (seq_t)k;
        }
    }
}
