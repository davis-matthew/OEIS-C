/*
 * A352458  2^k appears in the binary expansion of a(n) iff 2^k appears in the
 *          binary expansion of n and k AND n = 0 (where AND denotes the
 *          bitwise AND operator).
 * https://oeis.org/A352458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k: n has bit k set AND (k & n) == 0} 2^k
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the definition: 2^k appears in a(n) iff 2^k appears in n
   and k AND n = 0.                                               */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t n_orig = n;          /* keep original n for bit tests */
    seq_t result = 0;          /* accumulate answer */
    idx_t m = n_orig;          /* iterate over set bits of n */

    while (m) {
        /* find position of least significant set bit */
        idx_t k = 0;
        idx_t temp = m;
        while ((temp & 1ULL) == 0ULL) {
            temp >>= 1ULL;
            ++k;
        }

        /* include 2^k if (k & n_orig) == 0 */
        if ((k & n_orig) == 0ULL) {
            result = result + ((seq_t)1 << k);
        }

        /* clear the processed bit */
        m &= (m - 1);
    }

    return result;
}
