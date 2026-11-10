/*
 * A358475  Number of 7's that appeared by n-th step when constructing
 *          A030717.
 * https://oeis.org/A358475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   simulate the construction of A030717 row by row (row 1 = [1]; *
 *            each new row lists the frequencies of the distinct values
 *            already in the * first list, in ascending order of value); a(n)
 *            = number of 7's in the * first list after n rows
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 88 terms through n = 88 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    static uint64_t *cnt = NULL;     /* cnt[v] = occurrences of v in L1 */
    static size_t cnt_cap = 0;
    static uint64_t *res = NULL;     /* res[k] = count of 7's after k rows */
    static size_t res_len = 0, res_cap = 0;
    static uint64_t maxv = 0;        /* largest value present in L1 */
    static uint64_t *row = NULL;
    static size_t row_cap = 0;

    if (res == NULL) {
        cnt_cap = 1024;
        cnt = (uint64_t *)calloc(cnt_cap, sizeof(uint64_t));
        cnt[1] = 1;
        maxv = 1;
        res_cap = 128;
        res = (uint64_t *)malloc(res_cap * sizeof(uint64_t));
        res[0] = 0;
        res[1] = cnt[7];             /* after row 1 */
        res_len = 2;
    }

    while ((idx_t)res_len <= n) {
        /* build the new row from a snapshot of the frequencies */
        size_t rl = 0;
        if (row_cap < (size_t)maxv + 1) {
            row_cap = (size_t)maxv * 2 + 2;
            row = (uint64_t *)realloc(row, row_cap * sizeof(uint64_t));
        }
        for (uint64_t v = 1; v <= maxv; ++v)
            if (cnt[v] != 0) row[rl++] = cnt[v];
        /* add the row's values to L1 */
        for (size_t i = 0; i < rl; ++i) {
            uint64_t v = row[i];
            if (v >= cnt_cap) {
                size_t nc = cnt_cap;
                while (nc <= v) nc *= 2;
                cnt = (uint64_t *)realloc(cnt, nc * sizeof(uint64_t));
                memset(cnt + cnt_cap, 0, (nc - cnt_cap) * sizeof(uint64_t));
                cnt_cap = nc;
            }
            cnt[v]++;
            if (v > maxv) maxv = v;
        }
        if (res_len >= res_cap) {
            res_cap *= 2;
            res = (uint64_t *)realloc(res, res_cap * sizeof(uint64_t));
        }
        res[res_len++] = cnt[7];
    }

    return (seq_t)res[n];
}
