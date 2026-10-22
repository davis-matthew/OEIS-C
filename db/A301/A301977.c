/*
 * A301977  a(n) is the number of distinct positive numbers whose binary
 *          digits appear in order but not necessarily as consecutive digits
 *          in the binary representation of n.
 * https://oeis.org/A301977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 1274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
** Formula: a(n) = the number of distinct positive integers k whose binary
** representation appears as a subsequence of the binary representation of n,
** with the first digit of that subsequence being 1.
**
** The function enumerates all non‑empty subsequences of the binary digits of n.
** A subsequence is valid only if its first selected bit is 1; duplicates are
** removed by a simple boolean array indexed by the integer value of the
** subsequence.
** The algorithm runs in O(2^L · L) where L is the number of bits of n.
** For n ≤ 10000, L ≤ 14, so the computation is trivial.
*/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* Index 0 is outside the OEIS offset */

    /* Compute number of bits L of n */
    idx_t tmp = n;
    int L = 0;
    while (tmp) {
        ++L;
        tmp >>= 1;
    }

    /* Store individual bits of n, MSB first */
    uint8_t bits[64];
    for (int i = 0; i < L; ++i) {
        bits[i] = (uint8_t)((n >> (L - 1 - i)) & 1ULL);
    }

    /* Allocate array to remember which integer values have already been seen
       The maximum value that can be produced is n itself. */
    size_t max_val = (size_t)n;
    bool *seen = (bool *)calloc(max_val + 1, sizeof(bool));
    if (!seen) /* unlikely for the small n used in the tests */
        return (seq_t)0;

    size_t count = 0;

    /* Enumerate all non‑empty subsets of positions (2^L - 1 subsets) */
    if (L < 64) {
        size_t limit = ((size_t)1) << L;
        for (size_t mask = 1; mask < limit; ++mask) {
            bool firstFound = false;
            bool leadingOne  = true;
            seq_t val = 0;

            /* Walk through the bits in order MSB→LSB */
            for (int i = 0; i < L; ++i) {
                if ((mask >> (L - 1 - i)) & 1ULL) {
                    if (!firstFound) {
                        firstFound = true;
                        if (bits[i] == 0) {   /* first bit is 0 → invalid */
                            leadingOne = false;
                            break;
                        }
                    }
                    /* Build the integer represented by the subsequence */
                    val = (val << 1) | (seq_t)bits[i];
                }
            }
            if (!leadingOne) continue;

            /* Record the value if it has not been seen before */
            if (!seen[(size_t)val]) {
                seen[(size_t)val] = true;
                ++count;
            }
        }
    }

    free(seen);
    return (seq_t)count;
}
