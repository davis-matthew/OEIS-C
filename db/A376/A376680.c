/*
 * A376680  Run-lengths of first differences of composite numbers.
 * https://oeis.org/A376680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Rule: a(n) = run length of consecutive equal gaps between successive
 * composite numbers; the gaps are differences between consecutive composites,
 * and the run length is the number of consecutive equal gaps. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *runs = NULL;
static size_t run_cap = 0;
static size_t run_cnt = 0;

static bool init_done = false;
static idx_t last_comp;     /* last composite processed */
static idx_t prev_diff;     /* previous gap, (idx_t)-1 if none */
static idx_t cur_run;       /* length of the current run */
static idx_t next_n;        /* next integer to test for compositeness */

/* Ensure that the first `need` run lengths have been computed and stored. */
static void ensure_runs(idx_t need)
{
    if ((idx_t)run_cnt >= need)
        return;

    if (!init_done) {
        init_done = true;
        last_comp = 4;          /* first composite number */
        prev_diff = (idx_t)-1;  /* no previous gap yet */
        cur_run   = 0;
        next_n    = 5;          /* start checking from 5 */
    }

    while ((idx_t)run_cnt < need) {
        if (!is_prime(next_n)) {                      /* composite found */
            idx_t d = next_n - last_comp;             /* gap to previous composite */

            if (prev_diff == (idx_t)-1) {             /* first gap seen */
                prev_diff = d;
                cur_run = 1;
            } else if (d == prev_diff) {              /* continue current run */
                ++cur_run;
            } else {                                  /* run ended */
                if (run_cnt >= run_cap) {
                    size_t new_cap = run_cap ? run_cap * 2 : 16;
                    seq_t *ptr = (seq_t *)realloc(runs, new_cap * sizeof(seq_t));
                    if (!ptr) abort();
                    runs    = ptr;
                    run_cap = new_cap;
                }
                runs[run_cnt++] = (seq_t)cur_run;
                prev_diff = d;
                cur_run   = 1;
            }
            last_comp = next_n;
        }
        ++next_n;
    }
}

seq_t a(idx_t n)
{
    ensure_runs(n);
    return runs[n - 1];
}
