/*
 * A351014  Number of distinct runs in the n-th composition in standard order.
 * https://oeis.org/A351014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Implementation of OEIS A351014:
   a(n) = number of distinct runs (value & run length) in the standard-order
   composition derived from n.
   The construction follows the definition:
     - take the set of positions of 1's in the reversed binary expansion of n
       (positions counted from 1)
     - prepend 0
     - take first differences
     - reverse the resulting list to obtain the composition
   Finally, count the distinct (value,run_length) pairs in that composition. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* arrays large enough for the maximum number of set bits in 64‑bit n */
    int pos[65];   /* positions (1‑based) of set bits; pos[0] is implied 0   */
    int diff[65];  /* first differences of the positions including 0      */
    int m = 0;     /* number of set bits  (also length of diff)           */

    /* collect 1‑based positions of set bits in reversed binary order */
    for (idx_t bit = 0; bit < 64; ++bit) {
        if (n & ((idx_t)1 << bit))
            pos[++m] = (int)(bit + 1);
    }

    /* compute first differences (pos[0] is regarded as 0) */
    diff[0] = pos[1];              /* difference from 0 to first set bit   */
    for (int i = 1; i < m; ++i)
        diff[i] = pos[i + 1] - pos[i];

    /* count distinct runs in the reversed diff array (the composition) */
    seq_t distinct = 0;
    int seenVal[65];
    int seenLen[65];
    int seenCnt = 0;

    int curVal = diff[m - 1];
    int curLen = 1;

    for (int idx = m - 2; idx >= 0; --idx) {
        if (diff[idx] == curVal)
            ++curLen;
        else {
            int found = 0;
            for (int s = 0; s < seenCnt; ++s) {
                if (seenVal[s] == curVal && seenLen[s] == curLen) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                seenVal[seenCnt] = curVal;
                seenLen[seenCnt] = curLen;
                ++seenCnt;
                ++distinct;
            }
            curVal = diff[idx];
            curLen = 1;
        }
    }

    /* record the final run */
    int found = 0;
    for (int s = 0; s < seenCnt; ++s) {
        if (seenVal[s] == curVal && seenLen[s] == curLen) {
            found = 1;
            break;
        }
    }
    if (!found)
        ++distinct;

    return distinct;
}
