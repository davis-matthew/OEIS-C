/*
 * A266917  Add and multiply: distinct numbers (a+b) * (c+d) * (e+f) * (g+h) *
 *          (i+j) where a..j are permutations of 0..9.
 * https://oeis.org/A266917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   a(n) = nth smallest distinct value of (a+b)(c+d)(e+f)(g+h)(i+j)
 *            for all permutations of 0..9 (a,b,c,d,e,f,g,h,i,j are distinct
 *            digits)
 * Range:     u8 none, u16 n <= 400, u32 n <= 400, u64 n <= 400, u128 n <= 400,
 *            big any n
 * Validated: full, pass, 400 terms through n = 400 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* helper to generate all perfect matchings */
static void rec_match(unsigned used, int depth, int sums[5], seq_t *tmp, size_t *cnt)
{
    if (depth == 5) {                  /* all pairs chosen */
        seq_t prod = 1;
        for (int k = 0; k < 5; ++k)
            prod *= (seq_t)sums[k];
        tmp[(*cnt)++] = prod;
        return;
    }

    int i;
    for (i = 0; i < 10; ++i)
        if (!(used & (1 << i)))
            break;

    for (int j = i + 1; j < 10; ++j) {
        if (!(used & (1 << j))) {
            int newmask = used | (1 << i) | (1 << j);
            sums[depth] = i + j;
            rec_match(newmask, depth + 1, sums, tmp, cnt);
        }
    }
}

/* comparison for qsort on seq_t */
static int seq_t_cmp(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t*)p1;
    seq_t v2 = *(const seq_t*)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* global storage for computed values */
static seq_t  sorted_vals[400];
static size_t sorted_count = 0;
static bool   initialized = false;

/* initialization routine: compute all distinct products */
static void init_sequence(void)
{
    if (initialized) return;
    seq_t all[1000];          /* 945 matchings -> <1000 */
    size_t all_cnt = 0;
    int   sums[5];
    rec_match(0, 0, sums, all, &all_cnt);

    /* sort the raw products */
    mem_qsort(all, all_cnt, sizeof(seq_t), seq_t_cmp);

    /* deduplicate into sorted_vals */
    for (size_t i = 0; i < all_cnt; ++i) {
        if (i == 0 || all[i] != all[i - 1]) {
            sorted_vals[sorted_count++] = all[i];
            if (sorted_count == 400) /* we only need first 400 distinct values */
                break;
        }
    }
    initialized = true;
}

/* sequence accessor */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, n<1 undefined */
    init_sequence();
    if (n > sorted_count) return 0;
    return sorted_vals[n - 1];
}
