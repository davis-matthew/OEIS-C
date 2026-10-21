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
 * Helpers:   rec_match, seq_t_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/rec_match.h"
#include "../utils/seq_t_cmp.h"

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
    qsort(all, all_cnt, sizeof(seq_t), seq_t_cmp);

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
