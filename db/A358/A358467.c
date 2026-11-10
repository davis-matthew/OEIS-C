/*
 * A358467  Number of 1's that appeared in the n-th step when constructing
 *          A030717.
 * https://oeis.org/A358467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>=2, a(n) = number of 1's among the frequencies
 *            appended in the (n-1)-th step of the A030717 construction (list
 *            [1], then repeatedly append the frequencies of the distinct
 *            values in ascending order)
 * Range:     u8 n <= 100, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Validated: standard, pass, 97 terms through n = 727 (2026-11-10)
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
    static bool init = false;
    static size_t *cnt = NULL;   /* cnt[v] = occurrences of v in the first list */
    static size_t cap = 0;
    static size_t maxv = 0;
    static size_t *res = NULL;   /* res[k] = a(k+1) */
    static size_t res_len = 0, res_cap = 0;
    static size_t *tmp = NULL;
    static size_t tmp_cap = 0;

    if (!init) {
        cap = 64;
        cnt = (size_t *)calloc(cap, sizeof(size_t));
        res_cap = 64;
        res = (size_t *)calloc(res_cap, sizeof(size_t));
        tmp_cap = 64;
        tmp = (size_t *)calloc(tmp_cap, sizeof(size_t));
        cnt[1] = 1;
        maxv = 1;
        res[0] = 1;
        res_len = 1;
        init = true;
    }

    while (res_len < (size_t)n) {
        size_t len = 0, ones = 0, v, i;
        if (tmp_cap < maxv + 1) {
            while (tmp_cap < maxv + 1) tmp_cap <<= 1;
            free(tmp);
            tmp = (size_t *)calloc(tmp_cap, sizeof(size_t));
        }
        for (v = 1; v <= maxv; ++v) {
            if (cnt[v] > 0) {
                tmp[len++] = cnt[v];
                if (cnt[v] == 1) ones++;
            }
        }
        /* make sure cnt can hold the largest frequency */
        {
            size_t mf = 0;
            for (i = 0; i < len; ++i) if (tmp[i] > mf) mf = tmp[i];
            if (mf + 1 > cap) {
                size_t nc = cap;
                while (nc < mf + 1) nc <<= 1;
                size_t *nn = (size_t *)calloc(nc, sizeof(size_t));
                for (i = 0; i < cap; ++i) nn[i] = cnt[i];
                free(cnt);
                cnt = nn;
                cap = nc;
            }
            if (mf > maxv) maxv = mf;
        }
        for (i = 0; i < len; ++i) cnt[tmp[i]]++;

        if (res_len == res_cap) {
            size_t nc = res_cap * 2;
            size_t *nr = (size_t *)calloc(nc, sizeof(size_t));
            for (i = 0; i < res_len; ++i) nr[i] = res[i];
            free(res);
            res = nr;
            res_cap = nc;
        }
        res[res_len++] = ones;
    }
    return (seq_t)res[(size_t)n - 1];
}
