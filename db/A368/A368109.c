/*
 * A368109  Number of ways to choose a binary index of each binary index of n.
 * https://oeis.org/A368109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k∈A048793(n)} popcount(k) where A048793(n) is the set
 *            of positions of 1's in the reversed binary expansion of n (i.e.,
 *            the indices of set bits)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;      /* product over an empty set is 1 */
    idx_t i = 0;           /* current bit position, 0‑based */
    idx_t temp = n;

    while (temp != 0) {
        if (temp & 1ULL) {
            /* bit i is set -> binary index k = i+1 */
            idx_t k = i + 1;

            /* compute popcount(k) with a simple loop */
            uint64_t pc = 0;
            uint64_t t = k;
            while (t != 0) {
                pc += t & 1ULL;
                t >>= 1;
            }

            result *= (seq_t)pc;
        }
        temp >>= 1;
        ++i;
    }

    return result;
}
