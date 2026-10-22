/*
 * A319474  Number of partitions of n into exactly nine nonzero decimal
 *          palindromes.
 * https://oeis.org/A319474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = [x^n y^9] 1/Product_{j>=2} (1‑y*x^A002113(j))
 * Range:     u8 n <= 27, u16 n <= 304, u32 n <= 3783, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_palindrome_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count partitions of n into exactly nine non‑zero decimal palindromes. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_palindrome_int.h"

/* External integer types provided by the build system. */
extern seq_t a(idx_t n);

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* ---------- static memoisation state ---------- */
    static seq_t *dp   = NULL;        /* flattened array dp[sum][k], k=0..9 */
    static seq_t *res  = NULL;        /* res[i] = dp[i][9] */
    static idx_t  cur   = 0;          /* highest n that has been computed */
    static bool   init  = false;

    /* Recompute everything if we need a larger n. */
    if (!init || n > cur) {
        idx_t  new_max = n;
        size_t dp_size = (size_t)(new_max + 1) * 10;      /* (sum, k) pairs */
        size_t res_size = (size_t)(new_max + 1);

        /* Allocate new tables. Explicit casts required for C++. */
        seq_t *new_dp = (seq_t*)calloc(dp_size, sizeof(seq_t));
        seq_t *new_res = (seq_t*)calloc(res_size, sizeof(seq_t));
        if (!new_dp || !new_res) {
            /* Allocation failed – best effort return 0. */
            free(new_dp);
            free(new_res);
            return (seq_t)0;
        }

        /* ---------- build list of palindromes up to new_max ---------- */
        idx_t *pals = (idx_t*)malloc((size_t)(new_max + 1) * sizeof(idx_t));
        size_t pal_cnt = 0;
        for (idx_t i = 1; i <= new_max; ++i)
            if (is_palindrome_int(i))
                pals[pal_cnt++] = i;

        /* ---------- dynamic programming for partitions ---------- */
        new_dp[0 * 10 + 0] = (seq_t)1;      /* base: sum 0 with 0 parts */
        for (size_t pi = 0; pi < pal_cnt; ++pi) {
            idx_t p = pals[pi];
            for (idx_t sum = p; sum <= new_max; ++sum) {
                size_t sum_off = (size_t)sum * 10;
                for (int k = 1; k <= 9; ++k) {
                    size_t prev_off = ((size_t)(sum - p)) * 10 + (k - 1);
                    new_dp[sum_off + k] += new_dp[prev_off];
                }
            }
        }

        /* ---------- extract results for all n up to new_max ---------- */
        for (idx_t i = 0; i <= new_max; ++i)
            new_res[i] = new_dp[(size_t)i * 10 + 9];

        /* ---------- free old memory and store new tables ---------- */
        free(pals);
        free(dp);
        free(res);
        dp  = new_dp;
        res = new_res;
        cur = new_max;
        init = true;
    }

    /* ---------- return the stored answer ---------- */
    return res[n];
}
