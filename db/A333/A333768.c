/*
 * A333768  Minimum part of the n-th composition in standard order. a(0) = 0.
 * https://oeis.org/A333768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (shortest run of 0's after a 1 in binary representation
 *            of n) + 1
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   For n = 0, a(0) = 0. For n > 0, count zero runs that immediately follow a 1 (to the right)
 *   in the binary expansion, take the minimum length, and add 1. */
/*  Complexity: O(log n) per call, well below the time limits. */

#include <stdint.h>   /* for intptr_t, uint64_t, etc. */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Find position of the most significant 1 bit. */
    int msb = -1;
    for (int i = 63; i >= 0; --i) {
        if ((n >> i) & 1) {
            msb = i;
            break;
        }
    }

    /* Scan from msb down to 0, tracking runs of zeros after a 1. */
    unsigned char afterOne = 0;       /* 1 if previous bit was a 1 */
    unsigned char zerosInRun = 0;    /* count of zeros in current run */
    unsigned char minZeros = 255;    /* large initial value */

    for (int i = msb; i >= 0; --i) {
        unsigned char bit = (n >> i) & 1u;

        if (bit) {
            if (afterOne) {
                /* A run of zeros just finished. */
                if (zerosInRun < minZeros)
                    minZeros = zerosInRun;
                zerosInRun = 0;
            }
            afterOne = 1;   /* Start a new run after this 1 */
        } else { /* bit == 0 */
            if (afterOne)
                ++zerosInRun;   /* Part of the run after a 1 */
        }
    }

    /* Final run may end at the least significant bit. */
    if (afterOne && zerosInRun < minZeros)
        minZeros = zerosInRun;

    /* If for some reason no run was found (should not happen for n > 0), set to 0. */
    if (minZeros == 255)
        minZeros = 0;

    return (seq_t)(minZeros + 1);
}
