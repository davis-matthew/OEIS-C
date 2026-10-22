/*
 * A345809  Numbers that are the sum of ten cubes in exactly seven ways.
 * https://oeis.org/A345809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer that can be expressed as the sum of ten
 *            * positive cubes in exactly seven distinct unordered ways. * The
 *            implementation builds a dynamic table of counts of partitions
 *            into * exactly ten cubes using a DP over cube sizes. When the
 *            requested number * of terms has not yet been...
 * Range:     u8 none, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* -----  static state  ----- */

static seq_t *  seq        = NULL;   /* array of terms */
static size_t   seq_len    = 0;      /* number of terms stored   */
static idx_t    curr_Smax  = 0;      /* highest sum computed     */
static bool     initialized = false; /* whether seq has been built */

/* -----  helper: compute the sequence up to at least 'request_len' terms  ----- */

static void compute_until(idx_t request_len)
{
    /* If we already have enough terms, nothing to do. */
    if (initialized && seq_len >= (size_t)request_len)
        return;

    /* Free previous storage if we are recomputing. */
    if (seq != NULL) {
        free(seq);
        seq = NULL;
    }

    /* Initial bound.  If not yet initialized, start with 200 000. */
    idx_t Smax = initialized ? curr_Smax : 200000ULL;

    while (1) {
        /* Allocate DP table: 11 layers (0..10 cubes) × (Smax+1) sums. */
        idx_t stride = Smax + 1;
        uint8_t *dp_flat = (uint8_t *)calloc(11 * stride, sizeof(uint8_t));
        if (dp_flat == NULL)
            abort();   /* memory exhaustion */

        uint8_t *dp[11];
        for (int i = 0; i < 11; ++i)
            dp[i] = dp_flat + (size_t)i * stride;

        dp[0][0] = 1;   /* one way to make 0 with 0 cubes */

        /* Enumerate all cube sizes up to Smax. */
        for (uint64_t m = 1;; ++m) {
            seq_t cube = (seq_t)m * m * m;
            if (cube > (seq_t)Smax)
                break;

            idx_t cidx = (idx_t)cube;   /* cube as index in DP */

            for (int i = 1; i <= 10; ++i) {
                for (idx_t s = cidx; s <= Smax; ++s) {
                    uint8_t add = dp[i-1][s - cidx];
                    if (add == 0)
                        continue;
                    uint8_t newv = dp[i][s] + add;
                    if (newv > 7)   /* saturate at 8 for ≥8 ways */
                        newv = 8;
                    dp[i][s] = newv;
                }
            }
        }

        /* Count how many sums have exactly 7 representations. */
        size_t count = 0;
        for (idx_t s = 1; s <= Smax; ++s)
            if (dp[10][s] == 7)
                ++count;

        if (count >= (size_t)request_len) {
            /* Allocate and store the terms in strictly increasing order. */
            seq = (seq_t *)malloc(count * sizeof(seq_t));
            if (seq == NULL)
                abort();
            size_t idx = 0;
            for (idx_t s = 1; s <= Smax; ++s)
                if (dp[10][s] == 7)
                    seq[idx++] = (seq_t)s;

            seq_len      = count;
            curr_Smax    = Smax;
            initialized  = true;

            free(dp_flat);
            return;
        }

        /* Not enough terms yet: free DP and double the bound. */
        free(dp_flat);
        Smax *= 2;
    }
}

/* -----  public accessor  ----- */

seq_t a(idx_t n)
{
    /* OEIS indices start at 1. Undefined for n == 0. */
    if (n < 1)
        return 0;

    compute_until(n);          /* ensure we have at least n terms */

    return seq[n - 1];
}
