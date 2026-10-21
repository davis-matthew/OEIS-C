/*
 * A242217  Number of partitions of n into distinct Heegner numbers, cf.
 *          A003173.
 * https://oeis.org/A242217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,3,7,11,19,43,67,163} whose sum
 *            is n. Since the sum of all nine Heegner numbers is 316, a(n)=0
 *            for n>316
 * Range:     u8 n <= 400, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Counts for all sums 0..316.  The maximum count is 3, so the array fits in any seq_t. */
    static seq_t cache[317] = {0};
    static bool initialized = false;
    const idx_t H[] = {1,2,3,7,11,19,43,67,163};
    const size_t Hcnt = sizeof H / sizeof *H;

    if (!initialized) {
        /* Enumerate all subsets of the 9 Heegner numbers. */
        const idx_t maxMask = (idx_t)1 << Hcnt; /* 512 */
        for (idx_t mask = 0; mask < maxMask; ++mask) {
            idx_t sum = 0;
            for (size_t i = 0; i < Hcnt; ++i) {
                if (mask & ((idx_t)1 << i)) {
                    sum += H[i];
                }
            }
            if (sum <= 316) {
                /* cache[sum] holds the number of ways, fits in seq_t. */
                cache[(size_t)sum] += (seq_t)1;
            }
        }
        initialized = true;
    }

    if (n <= 316) {
        return cache[(size_t)n];
    } else {
        return (seq_t)0;
    }
}
