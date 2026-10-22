/*
 * A325919  Number of Motzkin meanders of length n with an odd number of humps
 *          and without peaks.
 * https://oeis.org/A325919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A325919 – Number of Motzkin meanders of length n
   with an odd number of humps and without peaks.
   The term is defined for n=0,1,2 as 0.
   We use dynamic programming over the current height,
   the last-step state (0=normal, 1=U waiting for H, 2=inside hump),
   and the parity of completed humps (0=even, 1=odd).
   Parity is flipped only when a hump is closed by a D step.
   All transitions respect the requirements:
   • never go below the x‑axis,
   • no UD peak,
   • a hump is U H+ D (at least one H between the U and its closing D).
   The DP enumerates all admissible walks of length n and sums those
   whose parity is odd at the end.  */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, free */
#include <string.h>   /* memset */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 2)
        return 0;

    /* Number of possible heights after n steps is n+1. */
    const size_t max_states = (size_t)(n + 1) * 3 * 2;   /* height * state * parity */

    seq_t *cur  = (seq_t *)malloc(max_states * sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(max_states * sizeof(seq_t));
    if (!cur || !next) {
        if (cur) free(cur);
        if (next) free(next);
        return 0;   /* allocation failed – safe fallback */
    }

    /* Zero out DP tables */
    memset(cur, 0, max_states * sizeof(seq_t));
    memset(next, 0, max_states * sizeof(seq_t));

    /* Helper macro for linearising (h, state, parity). */
    #define IDX(h, s, p) (((size_t)(h) * 3 + (size_t)(s)) << 1 | (size_t)(p))

    /* Initial condition: step 0, height 0, normal state, even parity. */
    cur[IDX(0, 0, 0)] = 1;

    for (idx_t step = 0; step < n; ++step) {
        /* Current reachable heights: 0 .. step. */
        size_t max_h = (size_t)step;
        /* Next reachable heights: 0 .. step+1. */
        size_t max_h_next = max_h + 1;

        /* Zero out only the part of 'next' that will be used. */
        size_t required_next = (max_h_next + 1) * 3 * 2;
        memset(next, 0, required_next * sizeof(seq_t));

        for (size_t h = 0; h <= max_h; ++h) {
            for (int s = 0; s < 3; ++s) {
                for (int p = 0; p < 2; ++p) {
                    seq_t val = cur[IDX(h, s, p)];
                    if (val == 0)
                        continue;

                    if (s == 0) {          /* normal state */
                        /* U -> pre‑hump */
                        next[IDX(h + 1, 1, p)] += val;
                        /* H -> stay normal */
                        next[IDX(h, 0, p)] += val;
                        /* D -> stay normal, if possible */
                        if (h > 0)
                            next[IDX(h - 1, 0, p)] += val;
                    } else if (s == 1) {   /* just had a U, no H yet */
                        /* H -> inside hump */
                        next[IDX(h, 2, p)] += val;
                        /* U -> stay pre‑hump (another U before the H) */
                        next[IDX(h + 1, 1, p)] += val;
                        /* D is forbidden (would be a peak) */
                    } else {                /* inside hump (at least one H already) */
                        /* H -> stay inside hump */
                        next[IDX(h, 2, p)] += val;
                        /* D -> close hump, flip parity */
                        if (h > 0)
                            next[IDX(h - 1, 0, p ^ 1)] += val;
                        /* U -> start a new potential hump (pre‑hump) */
                        next[IDX(h + 1, 1, p)] += val;
                    }
                }
            }
        }
        /* Swap cur and next for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* Sum over all final states with odd parity. */
    seq_t result = 0;
    for (size_t h = 0; h <= (size_t)n; ++h) {
        for (int s = 0; s < 3; ++s) {
            result += cur[IDX(h, s, 1)];   /* parity == 1 */
        }
    }

    free(cur);
    free(next);
    return result;
}
#undef IDX
