/*
 * A358472  Number of 4's that appeared by n-th step when constructing
 *          A030717.
 * https://oeis.org/A358472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4's in A030717 (first list L1) after n-1 rounds
 *            of the construction; computed by direct simulation of value
 *            counts. a(1)=0
 * Range:     u8 n <= 73, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static uint64_t *cnt = NULL;
    static size_t cap = 0;
    static size_t maxv = 0;
    static uint64_t *tmp = NULL;
    static size_t tmp_cap = 0;
    static uint64_t *hist = NULL;
    static size_t hist_len = 0, hist_cap = 0;

    if (cnt == NULL) {
        cap = 1024;
        cnt = (uint64_t *)calloc(cap, sizeof(uint64_t));
        tmp_cap = 1024;
        tmp = (uint64_t *)malloc(tmp_cap * sizeof(uint64_t));
        hist_cap = 1024;
        hist = (uint64_t *)malloc(hist_cap * sizeof(uint64_t));
        cnt[1] = 1;
        maxv = 1;
        hist[0] = 0;
        hist_len = 1;
    }

    while (hist_len < (size_t)n) {
        size_t k = 0;
        if (tmp_cap < maxv + 1) {
            while (tmp_cap < maxv + 1) tmp_cap <<= 1;
            tmp = (uint64_t *)realloc(tmp, tmp_cap * sizeof(uint64_t));
        }
        for (size_t v = 1; v <= maxv; ++v) {
            if (cnt[v] > 0) tmp[k++] = cnt[v];
        }
        for (size_t i = 0; i < k; ++i) {
            size_t f = (size_t)tmp[i];
            if (f >= cap) {
                size_t nc = cap;
                while (nc <= f) nc <<= 1;
                cnt = (uint64_t *)realloc(cnt, nc * sizeof(uint64_t));
                memset(cnt + cap, 0, (nc - cap) * sizeof(uint64_t));
                cap = nc;
            }
            cnt[f]++;
            if (f > maxv) maxv = f;
        }
        if (hist_len >= hist_cap) {
            hist_cap <<= 1;
            hist = (uint64_t *)realloc(hist, hist_cap * sizeof(uint64_t));
        }
        hist[hist_len++] = (cap > 4) ? cnt[4] : 0;
    }

    return (seq_t)hist[(size_t)n - 1];
}
