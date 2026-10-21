/*
 * A268994  Number of nX7 binary arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two not
 *          more than once.
 * https://oeis.org/A268994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×7 binary arrays with at most one pair of 1's
 *            horizontally or antidiagonally adjacent
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   popcount_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by dynamic programming over the rows.
   Only masks (7‑bit patterns) with at most one horizontal 11–pair
   are allowed.  A transition from a previous row mask to a
   current row mask is forbidden if it would create more than one
   adjacency (horizontal in the new row or down‑right di‑agonal
   between the two rows). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/popcount_uint.h"

#if defined(__GNUC__) || defined(__clang__)

#else

#endif

/* maximum number of 7‑bit masks */
#define MAX_MASKS 128

/* storage for valid masks and transition tables */
static int     valid_masks[MAX_MASKS];
static int     hcnt[MAX_MASKS];          /* horizontal 11 pairs of a mask  */
static int     acnt[MAX_MASKS][MAX_MASKS]; /* antidiagonal pairs between two masks*/
static size_t  nv = 0;                   /* number of valid masks */

/* initialise the tables once */
static void init_tables(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    nv = 0;
    for (int m = 0; m < MAX_MASKS; ++m)
    {
        int hc = popcount_uint((unsigned int)(m & (m >> 1)));
        if (hc > 1) continue;            /* reject masks with >1 horizontal pair */
        valid_masks[nv] = m;
        hcnt[nv]       = hc;
        ++nv;
    }

    /* pre‑compute antidiagonal adjacency counts (down‑right only) */
    for (int i = 0; i < (int)nv; ++i)
    {
        for (int j = 0; j < (int)nv; ++j)
        {
            acnt[i][j] = popcount_uint(valid_masks[i] & (valid_masks[j] >> 1));
        }
    }
}

/* --------------------------------------------------------------- */
/* Main sequence function                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* sequence is defined for n >= 1 */

    init_tables();                      /* ensure tables built */

    const size_t dp_size = nv * 2;      /* two flags: 0=unused, 1=used */

    seq_t *dp_cur  = (seq_t*)malloc(dp_size * sizeof(seq_t));
    seq_t *dp_next = (seq_t*)malloc(dp_size * sizeof(seq_t));
    if (!dp_cur || !dp_next) {
        exit(EXIT_FAILURE);
    }
    /* zero initialise (works for unsigned integer types and for most
       arbitrary‑precision integer clones that provide value‑semantics) */
    memset(dp_cur, 0, dp_size * sizeof(seq_t));
    memset(dp_next, 0, dp_size * sizeof(seq_t));

    /* initialise for the first row (n = 1) */
    for (size_t i = 0; i < nv; ++i)
    {
        if (hcnt[i] == 0)
            dp_cur[i * 2] = 1;            /* no adjacency used yet */
        else
            dp_cur[i * 2 + 1] = 1;        /* adjacency already used */
    }

    /* iterate rows 2 .. n */
    for (idx_t row = 1; row < n; ++row)
    {
        /* clear next‑row buffer */
        memset(dp_next, 0, dp_size * sizeof(seq_t));

        /* transitions from every previous mask to every current mask */
        for (int pi = 0; pi < (int)nv; ++pi)
        {
            seq_t val0 = dp_cur[pi * 2];
            seq_t val1 = dp_cur[pi * 2 + 1];
            if (!val0 && !val1) continue;

            for (int ci = 0; ci < (int)nv; ++ci)
            {
                int total_adj = hcnt[ci] + acnt[pi][ci];
                if (total_adj > 1) continue;      /* would exceed the limit */

                if (val0)
                {
                    if (total_adj == 0)
                        dp_next[ci * 2] += val0;         /* still unused */
                    else /* total_adj == 1 */
                        dp_next[ci * 2 + 1] += val0;     /* used now */
                }
                if (val1)
                {
                    if (total_adj == 0)
                        dp_next[ci * 2 + 1] += val1;     /* already used,
                                                          still stays used */
                }
            }
        }

        /* swap buffers for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* compute total for the final row */
    seq_t total = 0;
    for (int i = 0; i < (int)nv; ++i)
    {
        total += dp_cur[i * 2];
        total += dp_cur[i * 2 + 1];
    }

    free(dp_cur);
    free(dp_next);
    return total;
}
