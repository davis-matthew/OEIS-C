/*
 * A373414  Sum of the n-th maximal run of nonsquarefree numbers differing by
 *          one.
 * https://oeis.org/A373414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_squarefree, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A373414
 *  Sum of the n-th maximal run of consecutive nonsquarefree numbers.
 *
 *  The run consists of consecutive integers each of which is not
 *  square‑free.  The n-th run occurring in increasing order of
 *  appearance is summed.  The implementation records completed runs
 *  and memoises them so that calls with arbitrary n are answered
 *  efficiently.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t*, size_t, size_t) */

/* Ensure that *arr has room for at least ‘needed’ elements.
 * new_cap is chosen roughly double the previous capacity.
 * The function uses seq_realloc, which correctly handles
 * arbitrary‑precision seq_t objects in both C and C++.
 */
static inline void
ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap) ? *cap * 2 : 64;
    if (needed > new_cap) new_cap = needed;
    seq_t *tmp = seq_realloc(*arr, *cap, new_cap);
    *arr = tmp;
    *cap = new_cap;
}

/* --- sequence implementation ------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, guard */
    static seq_t *sums = NULL;    /* memoised run sums */
    static size_t sums_len = 0;   /* number of stored sums */
    static size_t sums_cap = 0;   /* capacity of sums array */
    static idx_t current_limit = 1; /* next integer to examine */
    static bool in_run = false;       /* flag: are we in a run? */
    static seq_t run_sum = 0;         /* running sum of current run */

    /* if we already have the requested term, answer immediately */
    if (sums_len >= n) return sums[n - 1];

    /* Extend the sequence until we have at least n sums stored. */
    while (sums_len < n) {
        bool squarefree = is_squarefree((seq_t)current_limit);

        if (!squarefree) {                /* current number is nonsquarefree */
            if (!in_run) {                /* start a new run */
                in_run  = true;
                run_sum = (seq_t)current_limit;
            } else {                      /* continue existing run */
                run_sum += (seq_t)current_limit;
            }
        } else {                           /* current number is squarefree */
            if (in_run) {                  /* run ends here */
                ensure_capacity(sums_len + 1, &sums_cap, &sums);
                sums[sums_len++] = run_sum;
                in_run = false;
                if (sums_len >= (size_t)n) {  /* enough sums collected? */
                    current_limit++;          /* next number for future calls */
                    return sums[n - 1];
                }
            }
        }
        current_limit++;
    }

    /* The loop above guarantees that sums_len >= n, so this line is
       reached only logically.  It returns the required value. */
    return sums[n - 1];
}
