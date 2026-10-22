/*
 * A345811  Numbers that are the sum of ten cubes in exactly nine ways.
 * https://oeis.org/A345811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Numbers that are the sum of ten cubes in exactly nine ways.
 *            Computed by counting unordered 10‐tuples of positive cubes with
 *            values 1^3,2^3,…,m^3 for all m^3 ≤ MAX_SUM. Then all sums that
 *            have exactly 9 such combinations are the terms of the sequence.
 *            The enumeration stops when the cubes exceed th...
 * Range:     u8 none, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_SUM 200000          /* Enough to capture all known terms. */
#define CUBES_BELOW(limit)      (limit)
#define NUM_CUBES(max_cube_root) (max_cube_root)

static bool init_done = false;
static seq_t *term_list = NULL;
static size_t term_count = 0;

seq_t a(idx_t n)
{
    /* Build the list of terms once. */
    if (!init_done) {
        /* ---------- choose cube limit ----------
           Find the largest integer m such that m^3 <= MAX_SUM. */
        size_t max_cube_root = 0;
        while ((uint64_t)(max_cube_root + 1) *
               (max_cube_root + 1) *
               (max_cube_root + 1) <= MAX_SUM)
            ++max_cube_root;

        /* Allocate array of cube values. */
        idx_t *cubes = (idx_t *)malloc(max_cube_root * sizeof(idx_t));
        for (size_t i = 0; i < max_cube_root; ++i)
            cubes[i] = (idx_t)(i + 1) * (i + 1) * (i + 1);

        /* DP arrays: dp[k][s] – number of unordered ways to choose k cubes
           summing to s. */
        size_t stride = MAX_SUM + 1;
        uint32_t *dp_prev = (uint32_t *)calloc(11 * stride, sizeof(uint32_t));
        uint32_t *dp_cur  = (uint32_t *)malloc( 11 * stride * sizeof(uint32_t));
        if (!dp_prev || !dp_cur) {
            free(cubes);
            exit(1);
        }
        dp_prev[0] = 1;                /* one way to choose 0 cubes to sum 0 */

        /* Process each cube value. */
        for (size_t cc = 0; cc < max_cube_root; ++cc) {
            /* Start with the current counts (choose 0 of this cube). */
            memcpy(dp_cur, dp_prev, 11 * stride * sizeof(uint32_t));

            idx_t val = cubes[cc];
            int icubef = (int)val;      /* safe: val <= MAX_SUM */

            for (int oldk = 0; oldk <= 10; ++oldk) {
                size_t base_old = (size_t)oldk * stride;
                for (size_t olds = 0; olds <= MAX_SUM; ++olds) {
                    uint32_t ways = dp_prev[base_old + olds];
                    if (!ways) continue;
                    int remaining = 10 - oldk;
                    for (int t = 1; t <= remaining; ++t) {
                        int newk = oldk + t;
                        int news = (int)olds + t * icubef;
                        if (news > MAX_SUM) break;
                        dp_cur[newk * stride + news] += ways;
                    }
                }
            }
            /* Swap dp_prev and dp_cur for next cube. */
            uint32_t *tmp = dp_prev;
            dp_prev = dp_cur;
            dp_cur  = tmp;
        }

        /* Extract all sums with exactly nine representations. */
        term_list  = NULL;
        term_count = 0;
        size_t cap   = 64;                     /* initial capacity */
        term_list    = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!term_list) {
            free(cubes);
            free(dp_prev);
            free(dp_cur);
            exit(1);
        }

        size_t stride10 = 10 * stride;
        for (size_t s = 0; s <= MAX_SUM; ++s) {
            if (dp_prev[stride10 + s] == 9) {
                if (term_count == cap) {
                    cap *= 2;
                    term_list = (seq_t *)realloc(term_list,
                                                 cap * sizeof(seq_t));
                    if (!term_list) {
                        free(cubes);
                        free(dp_prev);
                        free(dp_cur);
                        exit(1);
                    }
                }
                term_list[term_count++] = (seq_t)s;
            }
        }

        /* Clean up. */
        free(cubes);
        free(dp_prev);
        free(dp_cur);
        init_done = true;
    }

    /* Return the requested term. */
    if (n == 0) return 0;                       /* defined only for n>=1 */
    if ((idx_t)(n - 1) < term_count)
        return term_list[n - 1];
    return 0;                                   /* 0 if out of range */
}
