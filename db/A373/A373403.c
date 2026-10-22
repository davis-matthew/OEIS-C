/*
 * A373403  Length of the n-th maximal antirun of composite numbers differing
 *          by more than one.
 * https://oeis.org/A373403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = length of the n‑th maximal antirun of composite numbers differing by more than one. */
/* The function generates composites in increasing order, starts a new run whenever two consecutive composites differ by one, and records the length of each run. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"

static seq_t *runs = NULL;          /* array of run lengths */
static size_t run_cap = 0;          /* allocated capacity */
static size_t run_cnt = 0;          /* number of runs recorded */

static idx_t last_checked = 3;      /* numbers <4 are not composite */
static idx_t prev_composite = 0;    /* 0 indicates no previous composite yet */
static seq_t curr_len = 0;          /* length of ongoing run */

static void ensure_runs_cap(size_t needed)
{
    if (needed <= run_cap)
        return;
    size_t new_cap = run_cap ? run_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(runs, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    runs = tmp;
    run_cap = new_cap;
}

static void add_run(seq_t len)
{
    ensure_runs_cap(run_cnt + 1);
    runs[run_cnt++] = len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety for indices <1 */

    /* generate composites until we have at least n runs recorded */
    while (run_cnt < n)
    {
        ++last_checked;
        if (last_checked < 4) continue;          /* skip 0,1,2,3 */

        bool prime = is_prime_small((seq_t)last_checked);
        if (!prime)                             /* it's composite */
        {
            if (prev_composite == 0)            /* first composite encountered */
            {
                curr_len = 1;
            }
            else
            {
                idx_t diff = last_checked - prev_composite;
                if (diff == 1)                  /* run ends before current composite */
                {
                    add_run(curr_len);
                    curr_len = 1;              /* start new run with current composite */
                }
                else                            /* continue current run */
                {
                    curr_len++;
                }
            }
            prev_composite = last_checked;
        }
    }

    return runs[n - 1];   /* 1‑based indexing */
}
