/*
 * A368199  Number of times n appears as a term of A105774.
 * https://oeis.org/A368199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth term of the infinite word obtained by *
 *            iterating the substitution * 0 → 01, 1 → 2, 2 → 45, 3 → 67, 4 →
 *            67, 5 → 8, * 6 → 61, 7 → 6, 8 → 35 * and then applying the
 *            coding * 4,6 → 0; 0,2,3,7 → 1; 1,5,8 → 2. * This produces the
 *            sequence A368199 (offset 0)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Substitution mapping: child_list[symbol][child_index] */
    static const int child_list[9][2] = {
        {0, 1},   /* 0 → 01 */
        {2, -1},  /* 1 → 2 */
        {4, 5},   /* 2 → 45 */
        {6, 7},   /* 3 → 67 */
        {6, 7},   /* 4 → 67 */
        {8, -1},  /* 5 → 8 */
        {6, 1},   /* 6 → 61 */
        {6, -1},  /* 7 → 6 */
        {3, 5}    /* 8 → 35 */
    };
    static const int child_cnt[9] = {2, 1, 2, 2, 2, 1, 2, 1, 2};

    /* Maximum depth sufficient for any 64‑bit n; the array size is small. */
    static const int MAX_DEPTH = 64;
    static const idx_t INF = (idx_t)-1;          /* value signalling “infinite” */

    idx_t len[MAX_DEPTH + 1][9];
    /* At depth 0 each symbol occupies a single position. */
    for (int s = 0; s < 9; ++s) len[0][s] = 1;

    int depth = 0;
    /* Compute expansion lengths until the length of symbol 0 exceeds n
     * or the maximum depth is reached. */
    while (depth < MAX_DEPTH && len[depth][0] <= n)
    {
        int next = depth + 1;
        for (int s = 0; s < 9; ++s)
        {
            idx_t sum = 0;
            for (int i = 0; i < child_cnt[s]; ++i)
            {
                int child = child_list[s][i];
                idx_t child_len = len[depth][child];
                if (sum >= INF || child_len >= INF)
                    sum = INF;
                else
                {
                    idx_t t = sum + child_len;
                    /* Detect overflow: if addition wrapped around, set to INF. */
                    if (t < sum) sum = INF;
                    else sum = t;
                }
            }
            len[next][s] = sum;
        }
        ++depth;
    }

    /* Walk the substitution tree to locate the symbol at position n. */
    int cur_sym = 0;          /* start from the seed symbol 0 */
    idx_t k = n;
    int d = depth;
    while (d > 0)
    {
        for (int i = 0; i < child_cnt[cur_sym]; ++i)
        {
            int child = child_list[cur_sym][i];
            idx_t child_len = len[d - 1][child];
            if (k < child_len)
            {
                cur_sym = child;
                --d;
                break;
            }
            k -= child_len;
        }
    }

    /* Map the final symbol to the output value according to the coding. */
    switch (cur_sym)
    {
        case 4: case 6: return (seq_t)0;                   /* 4,6 → 0 */
        case 0: case 2: case 3: case 7: return (seq_t)1;    /* 0,2,3,7 → 1 */
        case 1: case 5: case 8: return (seq_t)2;            /* 1,5,8 → 2 */
        default: return (seq_t)0;                          /* safety fallback */
    }
}
