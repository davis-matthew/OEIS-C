/*
 * A353929  Number of distinct sums of runs (of 0's or 1's) in the binary
 *          expansion of n.
 * https://oeis.org/A353929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of distinct sums of runs of 0's or 1's
 * in the binary expansion of n.
 *
 * Implementation detail:
 *   - Process the binary representation from least‑significant to
 *     most‑significant bit.
 *   - Keep track of the current run value (0/1) and its length.
 *   - When a run ends, record its sum:
 *        * if the run is of 1's, store its length (sum equals length)
 *        * if the run is of 0's, note that a zero sum has appeared
 *   - Distinct sums are finally the set of recorded 1‑run lengths
 *     plus possibly the zero sum (if any 0‑run exists).
 *
 * Complexity: O(#bits) per call (≤ 64).
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: binary representation of 0 is a single run of 0's. */
    if (n == 0)
        return (seq_t)1;

    /* Boolean array to mark which run lengths of 1's have been seen.
       Index 1..64 (max run length for 64–bit numbers). */
    bool seen[65] = {0};

    bool hasZeroRun = false;        /* records whether any run of 0's exists */
    bool curBit = n & 1;            /* current bit value of the run being processed */
    uint64_t curLen = 1;            /* current run length */

    /* Process remaining bits of n, starting with the second least significant bit. */
    idx_t rem = n >> 1;             /* remaining bits to process */

    while (rem) {
        bool bit = rem & 1;
        if (bit == curBit) {
            /* Extend current run */
            ++curLen;
        } else {
            /* Finish current run */
            if (curBit) {
                /* Run of 1's: its sum equals its length */
                seen[(int)curLen] = true;
            } else {
                /* Run of 0's: sum is 0 */
                hasZeroRun = true;
            }
            /* Start new run */
            curBit = bit;
            curLen = 1;
        }
        rem >>= 1;
    }

    /* Process the final run */
    if (curBit) {
        seen[(int)curLen] = true;
    } else {
        hasZeroRun = true;
    }

    /* Count distinct sums: zero sum (if any) + distinct 1‑run lengths */
    seq_t distinct = hasZeroRun ? 1 : 0;
    for (int i = 1; i <= 64; ++i)
        if (seen[i])
            ++distinct;

    return distinct;
}
