/*
 * A358473  Number of 5's that appeared by n-th step when constructing
 *          A030717.
 * https://oeis.org/A358473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   simulation. Start with the first list [1] (step 1). At each
 *            later * step, append the frequencies of the list's distinct
 *            values, taken in * ascending order of value. a(n) = number of
 *            5's in the first list after * step n
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static uint64_t *cnt = NULL;   /* cnt[v] = occurrences of value v in L1 */
    static size_t cnt_cap = 0;
    static size_t maxv = 0;
    static uint64_t *res = NULL;   /* res[k-1] = number of 5's after step k */
    static size_t res_len = 0, res_cap = 0;
    static uint64_t *snap = NULL;
    static size_t snap_cap = 0;

    if (n == 0) return (seq_t)0;

    if (res == NULL) {
        cnt_cap = 64;
        cnt = (uint64_t *)calloc(cnt_cap, sizeof(uint64_t));
        cnt[1] = 1;
        maxv = 1;
        res_cap = 64;
        res = (uint64_t *)malloc(res_cap * sizeof(uint64_t));
        res[0] = 0;
        res_len = 1;
        snap_cap = 64;
        snap = (uint64_t *)malloc(snap_cap * sizeof(uint64_t));
    }

    while (res_len < (size_t)n) {
        size_t k = 0;
        for (size_t v = 1; v <= maxv; ++v) {
            if (cnt[v]) {
                if (k >= snap_cap) {
                    snap_cap *= 2;
                    snap = (uint64_t *)realloc(snap, snap_cap * sizeof(uint64_t));
                }
                snap[k++] = cnt[v];
            }
        }
        for (size_t i = 0; i < k; ++i) {
            size_t f = (size_t)snap[i];
            if (f >= cnt_cap) {
                size_t nc = cnt_cap;
                while (nc <= f) nc *= 2;
                cnt = (uint64_t *)realloc(cnt, nc * sizeof(uint64_t));
                for (size_t j = cnt_cap; j < nc; ++j) cnt[j] = 0;
                cnt_cap = nc;
            }
            cnt[f]++;
            if (f > maxv) maxv = f;
        }
        if (res_len >= res_cap) {
            res_cap *= 2;
            res = (uint64_t *)realloc(res, res_cap * sizeof(uint64_t));
        }
        res[res_len++] = (5 < cnt_cap) ? cnt[5] : 0;
    }

    return (seq_t)res[(size_t)n - 1];
}
