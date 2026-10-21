/*
 * A246733  Number of length n+4 0..4 arrays with no pair in any consecutive
 *          five terms totalling exactly 4.
 * https://oeis.org/A246733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 17, u64 n <= 40, u128 n <= 87,
 *            big any n
 * Helpers:   no_pair_sum4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + a(n-4) + 24*a(n-5) + 2*a(n-6) + 
 *                 5*a(n-7) + 11*a(n-8) - 8*a(n-10) - 2*a(n-11) + 3*a(n-12) +
 *                 a(n-15) + a(n-16)
 *
 * Initial values (offset 1): a(1) = 424, a(2) = 1096, a(3) = 2884, a(4) = 7612
 *
 * Computation: We model the count of admissible length‑m arrays by the
 * number of 4‑digit suffixes (a,b,c,d).  A state can be encoded as a
 * single integer in [0,625).  From a state we can append a digit x if
 * the 5‑tuple (a,b,c,d,x) contains no pair summing to 4.  For each
 * state and digit we pre‑compute the resulting next state (or
 * INVALID).  Then we extend the array length one step at a time,
 * updating the counts for each state.  The algorithm is O(n·5⁴)
 * which is fast for n up to 10 000.  The DP arrays are kept static so
 * that multiple calls in arbitrary order reuse the previously
 * computed information.  The function is valid C11 and C++17. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/no_pair_sum4.h"

/* constants */
#define DIGITS 5
#define STATE_COUNT ((size_t)625)      /* 5^4 */
static const idx_t INVALID = (idx_t)-1;

/* transition table, allocated once */
static idx_t *transitions = NULL;     /* size STATE_COUNT * DIGITS */

/* DP arrays, allocated once */
static seq_t *curr = NULL;            /* counts for current length */
static seq_t *next = NULL;            /* temporary buffer for next length */

/* current total array length (length of the array being counted) */
static size_t cur_len = 4;            /* before any initialization, length 4 */

/* helper: encode a 4‑tuple into a state index */
static inline idx_t encode4(int a, int b, int c, int d)
{
    return (idx_t)(a * 125 + b * 25 + c * 5 + d);
}

/* initialize the transition table */
static void init_transitions(void)
{
    if (transitions != NULL)
        return;
    transitions = (idx_t*)calloc(STATE_COUNT * DIGITS, sizeof(idx_t));
    if (transitions == NULL)
        exit(1);   /* allocation failure */

    int a, b, c, d, x, w[5];
    for (idx_t state = 0; state < STATE_COUNT; ++state) {
        /* decode to a,b,c,d */
        d = state % 5;
        int rem = (int)(state / 5);
        c = rem % 5;
        rem /= 5;
        b = rem % 5;
        rem /= 5;
        a = rem;     /* rem < 5 */

        for (x = 0; x < DIGITS; ++x) {
            w[0] = a; w[1] = b; w[2] = c; w[3] = d; w[4] = x;
            if (no_pair_sum4(w))
                transitions[state * DIGITS + x] = encode4(b, c, d, x);
            else
                transitions[state * DIGITS + x] = INVALID;
        }
    }
}

/* initialize the DP arrays for length 5 */
static void init_len5(void)
{
    if (curr == NULL) {
        curr = (seq_t*)calloc(STATE_COUNT, sizeof(seq_t));
        next = (seq_t*)calloc(STATE_COUNT, sizeof(seq_t));
        if (curr == NULL || next == NULL)
            exit(1);   /* allocation failure */
    }

    int a0, b0, c0, d0, e0, w[5];
    for (a0 = 0; a0 < DIGITS; ++a0)
        for (b0 = 0; b0 < DIGITS; ++b0)
            for (c0 = 0; c0 < DIGITS; ++c0)
                for (d0 = 0; d0 < DIGITS; ++d0)
                    for (e0 = 0; e0 < DIGITS; ++e0) {
                        w[0] = a0; w[1] = b0; w[2] = c0; w[3] = d0; w[4] = e0;
                        if (no_pair_sum4(w)) {
                            idx_t st = encode4(b0, c0, d0, e0);
                            curr[st] += (seq_t)1;
                        }
                    }
    cur_len = 5;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;       /* offset is 1; this case is not used */

    init_transitions();

    /* initialise DP arrays if necessary */
    if (curr == NULL)
        init_len5();

    size_t target_len = (size_t)n + 4u;

    /* extend the DP to the desired length */
    while (cur_len < target_len) {
        /* zero the next buffer */
        for (size_t i = 0; i < STATE_COUNT; ++i)
            next[i] = (seq_t)0;

        /* transition from current to next */
        for (size_t s = 0; s < STATE_COUNT; ++s) {
            seq_t val = curr[s];
            if (val == 0)
                continue;
            for (int x = 0; x < DIGITS; ++x) {
                idx_t ns = transitions[s * DIGITS + x];
                if (ns != INVALID)
                    next[ns] += val;
            }
        }

        /* swap buffers */
        seq_t *tmp = curr;
        curr = next;
        next = tmp;

        ++cur_len;
    }

    /* sum all counts to obtain a(n) */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < STATE_COUNT; ++i)
        result += curr[i];
    return result;
}
