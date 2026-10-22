/*
 * A345784  Numbers that are the sum of eight cubes in exactly two ways.
 * https://oeis.org/A345784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 173, u32 n <= 173, u64 n <= 173, u128 n <= 173,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds the sequence up to the required term by dynamic
   programming over cube values, counting representations with up to 8 cubes
   and capping counts at 3. The DP table is rebuilt with an increased bound
   until enough terms are found.  It uses the externally defined cmp_seq_t
   routine for sorting. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Required comparison routine supplied by the OEIS build. */
# include "../utils/cmp_seq_t.h"

/* Global storage for the sequence. */
static seq_t *g_terms = NULL;
static size_t g_len   = 0;

/* Fast integer cube root: floor(cuberoot(n)). Works for 64‑bit unsigned values. */
static inline idx_t int_cube_root(idx_t n)
{
    idx_t low = 0, high = ((idx_t)1 << 22);   /* 2^22^3 > 2^64 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= n) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Build the sequence up to at least target_n terms.  The table is rebuilt
   with an increasing limit Smax until enough terms are found. */
static void build_sequence(idx_t target_n)
{
    idx_t Smax = 200000;                     /* starting bound, extended as needed */
    while (1) {
        size_t sz = (size_t)Smax + 1;
        /* Allocate 9 rows (0…8 cubes) of DP counts, capped at 3. */
        uint8_t **dp = (uint8_t **)malloc(9 * sizeof(uint8_t *));
        for (int k = 0; k <= 8; ++k)
            dp[k] = (uint8_t *)calloc(sz, 1);          /* zeroed */
        dp[0][0] = 1;                                 /* one way to make zero with zero cubes */

        idx_t maxRoot = int_cube_root(Smax);
        for (idx_t r = 1; r <= maxRoot; ++r) {
            size_t cube = (size_t)r * r * r;          /* cube value */
            for (size_t s = cube; s <= Smax; ++s) {
                for (int k = 1; k <= 8; ++k) {
                    uint8_t add = dp[k-1][s - cube];
                    if (!add) continue;
                    uint8_t cur  = dp[k][s];
                    uint8_t newcnt = cur + add;
                    if (newcnt > 3u) newcnt = 3u;
                    dp[k][s] = newcnt;
                }
            }
        }

        /* Collect all sums that have exactly two representations. */
        size_t cap = 32;
        seq_t *local = (seq_t *)malloc(cap * sizeof(seq_t));
        size_t len = 0;
        for (size_t s = 0; s <= Smax; ++s) {
            if (dp[8][s] == 2) {
                if (len >= cap) {
                    cap <<= 1;
                    local = (seq_t *)realloc(local, cap * sizeof(seq_t));
                }
                local[len++] = (seq_t)s;
            }
        }

        /* Free the DP tables. */
        for (int k = 0; k <= 8; ++k) free(dp[k]);
        free(dp);

        if (len < (size_t)target_n) {
            free(local);
            Smax <<= 1;                     /* double the bound and try again */
            continue;
        }

        /* Sort the collected terms so that a(1) is the smallest. */
        qsort(local, len, sizeof(seq_t), cmp_seq_t);

        /* Replace global sequence storage. */
        free(g_terms);
        g_terms = local;
        g_len   = len;
        break;
    }
}

/* Public entry point: returns the n‑th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for offset 0 */
    if (g_len < (size_t)n) build_sequence(n);   /* build if needed */
    return g_terms[(size_t)n - 1];
}
