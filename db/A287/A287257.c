/*
 * A287257  Number of set partitions of [n] such that all absolute differences
 *          between least elements of consecutive blocks are <= seven.
 * https://oeis.org/A287257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of [n] with all absolute differences
 *            between consecutive block minima <= 7. Computation uses a
 *            dynamic programming over the number of blocks and the distance
 *            from the current element to the last block minimum. Let
 *            dp[cnt][dist] be the number of partitions of the set {1...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 25, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;                /* a(0) = 1 */

    /* number of possible block counts + 1 (for index 0) */
    const size_t maxCnt = (size_t)n + 1;
    const size_t width  = 8;            /* distance values 0..7 */
    const size_t arrSize = (maxCnt + 1) * width;   /* include room for cnt=n */

#if defined(__cplusplus)
    /* C++: use new[] to get value-initialized objects */
    seq_t *dp_prev = new seq_t[arrSize];
    seq_t *dp_cur  = new seq_t[arrSize];
#else
    /* C: use calloc which zeroes the memory (seq_t is a plain integer type) */
    seq_t *dp_prev = (seq_t*)calloc(arrSize, sizeof(seq_t));
    seq_t *dp_cur  = (seq_t*)calloc(arrSize, sizeof(seq_t));
#endif

    /* initialize for pos = 1 : one block, distance 0 */
    dp_prev[1 * width + 0] = (seq_t)1;

    /* iterate over positions 2 .. n */
    for (idx_t pos = 2; pos <= n; ++pos) {
        const size_t maxCntPrev = (size_t)pos - 1;
        const size_t curSize = pos * width;

        /* zero dp_cur of required size */
#if defined(__cplusplus)
        std::fill(dp_cur, dp_cur + curSize, (seq_t)0);
#else
        size_t i;
        for (i = 0; i < curSize; ++i)
            dp_cur[i] = (seq_t)0;
#endif

        /* transitions from dp_prev to dp_cur */
        for (size_t cnt = 1; cnt <= maxCntPrev; ++cnt) {
            size_t basePrev = cnt * width;
            size_t baseAdd  = basePrev;          /* stay in same cnt */
            size_t baseNew  = (cnt + 1) * width; /* new block */

            for (size_t d = 0; d < 8; ++d) {
                seq_t val = dp_prev[basePrev + d];
                if (!val) continue;                /* skip zero states */

                /* add to existing block: keep cnt, increase distance */
                size_t new_d = d + 1;
                if (new_d >= 8) new_d = 7;
                dp_cur[baseAdd + new_d] += val * (seq_t)cnt;

                /* start a new block (only if distance <= 6) */
                if (d <= 6) {
                    dp_cur[baseNew + 0] += val;
                }
            }
        }

        /* swap dp_prev and dp_cur for next iteration */
#if defined(__cplusplus)
        std::swap(dp_prev, dp_cur);
#else
        seq_t *tmp = dp_prev;
        dp_prev     = dp_cur;
        dp_cur      = tmp;
#endif
    }

    /* sum all states in dp_prev to obtain a(n) */
    seq_t result = (seq_t)0;
    for (size_t cnt = 1; cnt <= (size_t)n; ++cnt) {
        size_t base = cnt * width;
        for (size_t d = 0; d < 8; ++d)
            result += dp_prev[base + d];
    }

#if defined(__cplusplus)
    delete[] dp_prev;
    delete[] dp_cur;
#else
    free(dp_prev);
    free(dp_cur);
#endif

    return result;
}
