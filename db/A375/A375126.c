/*
 * A375126  Strictly decreasing run-leader transformation for standard
 *          compositions.
 * https://oeis.org/A375126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index of the composition of leaders of strictly
 *            decreasing runs of the n-th composition in standard (graded
 *            reverse‑lexicographic) order
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* 1. Find positions of 1-bits in ascending order (LSB=1). */
    idx_t pos[64];
    idx_t posCnt = 0;
    idx_t tmp = n;
    idx_t bit = 1;
    while (tmp) {
        if (tmp & 1)
            pos[posCnt++] = bit;
        tmp >>= 1;
        bit++;
    }

    /* 2. Compute the part differences. */
    idx_t diff[64];
    idx_t diffCnt = 0;
    idx_t prev = 0;
    for (idx_t i = 0; i < posCnt; ++i) {
        diff[diffCnt++] = pos[i] - prev;
        prev = pos[i];
    }

    /* 3. Reverse the differences to obtain the composition. */
    idx_t comp[64];
    idx_t compCnt = 0;
    for (idx_t i = diffCnt; i > 0; --i)
        comp[compCnt++] = diff[i - 1];

    /* 4. Extract leaders of strictly decreasing runs. */
    idx_t leaders[64];
    idx_t leadCnt = 0;
    idx_t i = 0;
    while (i < compCnt) {
        leaders[leadCnt++] = comp[i];
        idx_t j = i + 1;
        while (j < compCnt && comp[j] < comp[j - 1])
            ++j;
        i = j;
    }

    /* 5. Compute the index of the leaders composition. */
    idx_t sum = 0;
    idx_t index = 0;
    for (idx_t k = leadCnt; k > 0; --k) {
        sum += leaders[k - 1];
        index += ((idx_t)1 << (sum - 1));
    }

    return (seq_t)index;
}
