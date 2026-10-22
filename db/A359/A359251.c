/*
 * A359251  Sum of terms in an odd-even expansion of n.
 * https://oeis.org/A359251
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of all terms produced by the odd–even expansion of n,
 *            excluding the initial n itself. The expansion is performed
 *            iteratively. The maximum row length required for n < 10000 is
 *            below 200, so a static array is sufficient, avoiding any dynamic
 *            allocation
 * Range:     u8 n <= 31, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)2) return 0;   /* offset is 2; behaviour for n<2 is irrelevant */

    /* A generous upper bound on the row length needed for n up to 10000. */
    const size_t MAX_LEN = 256;
    seq_t cur[MAX_LEN];
    seq_t next[MAX_LEN];

    /* initialise the first row with the root value n */
    size_t cur_len = 1;
    cur[0] = (seq_t)n;

    seq_t total = 0;

    for (;;)
    {
        /* check if any element can still expand (>= 2) */
        bool has_expand = false;
        for (size_t i = 0; i < cur_len; ++i)
            if (cur[i] >= (seq_t)2) { has_expand = true; break; }

        if (!has_expand) break;               /* expansion finished */

        /* new row has length cur_len + 2 */
        size_t next_len = cur_len + 2;
        for (size_t i = 0; i < next_len; ++i) next[i] = 0;

        seq_t row_sum = 0;

        for (size_t i = 0; i < cur_len; ++i)
        {
            seq_t v = cur[i];
            if (v < (seq_t)2) continue;       /* 0 or 1 produce nothing */

            if ((v & (seq_t)1) == 0)          /* even: 2b */
            {
                seq_t b = v >> 1;             /* b = v / 2 */
                next[i]     += b;
                next[i + 2] += b;
                row_sum += b;
                row_sum += b;
            }
            else                               /* odd: 2c+1 */
            {
                seq_t c = (v - (seq_t)1) >> 1;/* c = (v-1)/2 */
                next[i]     += c;
                next[i + 1] += (seq_t)1;
                next[i + 2] += c;
                row_sum += c;
                row_sum += (seq_t)1;
                row_sum += c;
            }
        }

        total += row_sum;

        /* move the new row into cur for the next iteration */
        for (size_t i = 0; i < next_len; ++i) cur[i] = next[i];
        cur_len = next_len;
    }

    return total;
}
