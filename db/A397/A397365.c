/*
 * A397365  Number of jumps of increasing length 1, 2, 3, ... to reach n
 *          starting from 0 and jumping to left or right but landing only on
 *          nonnegative numbers.
 * https://oeis.org/A397365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the minimal number of increasing jumps 1,2,3,... *
 *            required to reach n from 0 while never landing on a negative
 *            number. * Computed by dynamic programming: for each step i, keep
 *            all reachable * positions and record the first step count when
 *            each position appears
 * Range:     u8 n <= 9479, u16 n <= 9479, u32 n <= 9479, u64 n <= 9479,
 *            u128 n <= 9479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static state that persists between calls */
    static seq_t *min_steps   = NULL;   /* minimal steps for each position */
    static size_t min_steps_sz = 0;     /* size of min_steps array          */
    static char *curr         = NULL;   /* reachable positions for current step */
    static size_t curr_sz     = 0;       /* size of curr array                */
    static idx_t  cur_steps   = 0;       /* number of steps performed so far   */
    const seq_t UNKNOWN = (seq_t)(-1);   /* sentinel for "unknown"            */

    /* initialise on the first call */
    if (min_steps == NULL) {
        min_steps_sz = 1;
        min_steps = (seq_t *)malloc(min_steps_sz * sizeof(seq_t));
        min_steps[0] = 0;                    /* 0 steps to reach position 0      */

        curr_sz = 1;
        curr = (char *)malloc(curr_sz * sizeof(char));
        curr[0] = 1;                         /* position 0 reachable after 0 steps */
        cur_steps = 0;
    }

    /* extend computation until we know the answer for n */
    while (n >= min_steps_sz || min_steps[n] == UNKNOWN) {
        /* proceed to the next jump length */
        cur_steps++;

        /* new reachable positions after this jump */
        size_t new_sz = curr_sz + cur_steps;
        char *next = (char *)calloc(new_sz, sizeof(char)); /* zero‑filled */

        /* build next reachable set from current */
        for (size_t i = 0; i < curr_sz; ++i) {
            if (curr[i]) {
                /* jump right: i + cur_steps */
                next[i + cur_steps] = 1;
                /* jump left: i - cur_steps (only if i >= cur_steps) */
                if (i >= (size_t)cur_steps) {
                    next[i - cur_steps] = 1;
                }
            }
        }

        /* enlarge min_steps array if needed, filling new slots with UNKNOWN */
        if (new_sz > min_steps_sz) {
            size_t old_sz = min_steps_sz;
            min_steps_sz = new_sz;
            seq_t *tmp = (seq_t *)realloc(min_steps,
                                          min_steps_sz * sizeof(seq_t));
            min_steps = tmp;
            for (size_t i = old_sz; i < min_steps_sz; ++i) {
                min_steps[i] = UNKNOWN;
            }
        }

        /* record minimal step count for newly reachable positions */
        for (size_t i = 0; i < new_sz; ++i) {
            if (next[i] && min_steps[i] == UNKNOWN) {
                min_steps[i] = (seq_t)cur_steps;
            }
        }

        /* advance to next iteration */
        free(curr);
        curr = next;
        curr_sz = new_sz;
    }

    return min_steps[n];
}
