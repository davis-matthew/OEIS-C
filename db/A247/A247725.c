/*
 * A247725  Number of length n+3 0..7 arrays with no disjoint pairs in any
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..7 arrays of length n+3 with no disjoint
 *            pairs in any consecutive four terms having the same sum.
 *            Computed via dynamic programming over the last three entries
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 20, u128 n <= 44,
 *            big any n
 * Helpers:   triple_decode, triple_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/triple_decode.h"
#include "../utils/triple_index.h"

/* Maximum n we might handle.  The implementation will grow
   storage as needed, so this is just a starting hint. */
#define INITIAL_MAX_N 1000

/* Number of possible values for an entry (0..7). */
#define VALS 8
/* Number of possible triples: VALS^3 = 512. */
#define STATES (VALS*VALS*VALS)

/* Precompute, for each state and next value, whether the
   quadruple satisfies the “no equal disjoint pair sums” condition. */
static bool allowed[STATES][VALS] = {0};

/* Indicate whether allowed table has been built. */
static bool allowed_built = false;

static void build_allowed(void) {
    if (allowed_built) return;
    for (unsigned ai = 0; ai < VALS; ++ai) {
        for (unsigned bi = 0; bi < VALS; ++bi) {
            for (unsigned ci = 0; ci < VALS; ++ci) {
                unsigned idx = triple_index(ai, bi, ci);
                for (unsigned xi = 0; xi < VALS; ++xi) {
                    /* Check the three possible disjoint pair sums in (a,b,c,x). */
                    unsigned s1 = ai + bi;    /* a+b */
                    unsigned s2 = ci + xi;    /* c+x */
                    if (s1 == s2) { allowed[idx][xi] = false; continue; }

                    unsigned s3 = ai + ci;    /* a+c */
                    unsigned s4 = bi + xi;    /* b+x */
                    if (s3 == s4) { allowed[idx][xi] = false; continue; }

                    unsigned s5 = ai + xi;    /* a+x */
                    unsigned s6 = bi + ci;    /* b+c */
                    if (s5 == s6) { allowed[idx][xi] = false; continue; }

                    allowed[idx][xi] = true;
                }
            }
        }
    }
    allowed_built = true;
}

/* The following static variables hold the DP tables and results. */
static seq_t *results = NULL;            /* results[n] = a(n), n >= 0 */
static size_t results_capacity = 0;      /* size of allocated results array */

/* DP arrays for one step. */
static seq_t dp_cur[STATES];
static seq_t dp_next[STATES];

/* Ensures that the results array is large enough to hold up to n. */
static void ensure_results_capacity(size_t n) {
    if (results_capacity > n) return;
    size_t new_cap = results_capacity ? (results_capacity << 1) : INITIAL_MAX_N;
    if (new_cap <= n) new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
    results = tmp;
    results_capacity = new_cap;
}

/* Compute the DP up to the required n (if not already done). */
static void compute_up_to(size_t n) {
    ensure_results_capacity(n);
    if (n == 0) {
        /* Offset is 1; a(0) is unused but we initialize to 0. */
        if (results[0] == 0) results[0] = (seq_t)0;
        return;
    }
    /* If we have already computed up to this n, nothing to do. */
    static size_t computed_max = 0;
    if (computed_max >= n) return;

    build_allowed();

    /* Initialize dp_cur for the first 3 entries: all 512 states have count 1. */
    for (unsigned i = 0; i < STATES; ++i) dp_cur[i] = (seq_t)1;

    /* We have already computed 0 additions (length 3).  The first stored
       result corresponds to n=0, which is not used.  We start storing
       from step=1 (adding the 4th element). */
    for (size_t step = 1; step <= n; ++step) {
        /* Zero dp_next */
        for (unsigned i = 0; i < STATES; ++i) dp_next[i] = (seq_t)0;
        /* Transition */
        for (unsigned s = 0; s < STATES; ++s) {
            seq_t cur_val = dp_cur[s];
            if (cur_val == 0) continue;
            for (unsigned x = 0; x < VALS; ++x) {
                if (!allowed[s][x]) continue;
                unsigned new_s = ((s >> 3) | (x << 6)); /* shift left by one element and insert x at low bits */
                /* new_s = triple_index(b,c,x) where (a,b,c)=decode(s) */
                dp_next[new_s] += cur_val;
            }
        }
        /* Sum all counts to get a(step) */
        seq_t total = (seq_t)0;
        for (unsigned i = 0; i < STATES; ++i) total += dp_next[i];
        results[step] = total;

        /* Prepare for next iteration */
        for (unsigned i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }
    computed_max = n;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;               /* offset is 1 */
    size_t idx = (size_t)n;
    compute_up_to(idx);
    return results[ idx ];
}
