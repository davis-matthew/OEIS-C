/*
 * A247925  Number of length n+4 0..6 arrays with some disjoint pairs in every
 *          consecutive five terms having the same sum.
 * https://oeis.org/A247925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 8, u64 n <= 22, u128 n <= 49,
 *            big any n
 * Helpers:   window_good
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A247925 via DP over the last 4 entries.
   a(n) = number of length n+4 arrays over {0..6}
          such that every consecutive 5 terms contain two disjoint
          pairs with equal sum.  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/window_good.h"

#define ALPHA 7               /* alphabet size 0..6 */
#define STATE_SIZE (ALPHA*ALPHA*ALPHA*ALPHA) /* 7^4 = 2401 */

/* Precompute transitions: next_state[s][v] = new state after appending v,
   or -1 if the 5‑tuple is not valid.  */
static int next_state[STATE_SIZE][ALPHA] = {0};

/* Memoised answers: ans[n] holds a(n) for already computed n.  */
static seq_t *ans = NULL;         /* dynamic array of results */
static size_t ans_cap = 0;        /* capacity of ans (currently max_n_seen+1) */

static seq_t *dp_cur = NULL;      /* counts for current length (last 4 entries) */
static seq_t *dp_next = NULL;     /* buffer for next step */

/* Build the transition table once. */
static void init_transitions(void) {
    int a, b, c, d, v;
    int idx, t;
    int window[5];
    bool good;

    for (a = 0; a < ALPHA; a++)
        for (b = 0; b < ALPHA; b++)
            for (c = 0; c < ALPHA; c++)
                for (d = 0; d < ALPHA; d++) {
                    idx = (((a * ALPHA + b) * ALPHA + c) * ALPHA + d);
                    for (v = 0; v < ALPHA; v++) {
                        window[0] = a; window[1] = b; window[2] = c;
                        window[3] = d; window[4] = v;
                        good = window_good(window);
                        if (good) {
                            /* new state is (b,c,d,v) */
                            t = (((b * ALPHA + c) * ALPHA + d) * ALPHA + v);
                            next_state[idx][v] = t;
                        } else
                            next_state[idx][v] = -1;
                    }
                }
}

/* Initialize DP arrays and first answer a(1). */
static void init_dp(void) {
    size_t i;
    /* Allocate DP arrays. */
    dp_cur = (seq_t*)calloc(STATE_SIZE, sizeof(seq_t));
    dp_next = (seq_t*)calloc(STATE_SIZE, sizeof(seq_t));

    /* Enumerate all 5‑tuples to fill dp_cur for length 5 (n=1). */
    int x[5];
    for (x[0] = 0; x[0] < ALPHA; x[0]++)
    for (x[1] = 0; x[1] < ALPHA; x[1]++)
    for (x[2] = 0; x[2] < ALPHA; x[2]++)
    for (x[3] = 0; x[3] < ALPHA; x[3]++)
    for (x[4] = 0; x[4] < ALPHA; x[4]++) {
        if (!window_good(x))
            continue;
        /* State is last 4 entries: x[1..4] */
        int idx = (((x[1] * ALPHA + x[2]) * ALPHA + x[3]) * ALPHA + x[4]);
        dp_cur[idx] += 1;
    }

    /* Store a(1). */
    ans_cap = 2;       /* we need indices 0..1 */
    ans = (seq_t*)malloc(ans_cap * sizeof(seq_t));
    ans[0] = 0;        /* unused */
    seq_t sum = 0;
    for (i = 0; i < STATE_SIZE; i++)
        sum += dp_cur[i];
    ans[1] = sum;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* Ensure the module is initialized once. */
    static bool initialized = false;
    if (!initialized) {
        init_transitions();
        init_dp();
        initialized = true;
    }

    if (n == 0) {
        return 0;
    }

    /* Extend memoisation up to requested n if necessary. */
    if ((size_t)n >= ans_cap) {
        size_t new_cap = n + 1;
        ans = (seq_t*)realloc(ans, new_cap * sizeof(seq_t));
        /* If reallocation fails, the program will terminate. */
        seq_t *old_dp_cur = dp_cur;
        /* dp_cur already has counts for length corresponding to current max. */
        /* Compute for previous maximum index: find current max computed */
        size_t curr_max = ans_cap - 1;        /* last computed n */
        /* While extending from curr_max+1 to n */
        size_t i, s;
        for (size_t target = curr_max + 1; target <= (size_t)n; target++) {
            /* Zero dp_next */
            for (i = 0; i < STATE_SIZE; i++)
                dp_next[i] = 0;
            /* Transition */
            for (s = 0; s < STATE_SIZE; s++) {
                seq_t val = old_dp_cur[s];
                if (val == 0)
                    continue;
                int v;
                for (v = 0; v < ALPHA; v++) {
                    int ns = next_state[s][v];
                    if (ns >= 0)
                        dp_next[ns] += val;
                }
            }
            /* Swap dp arrays */
            seq_t *tmp = old_dp_cur;
            old_dp_cur = dp_next;
            dp_next = tmp;
            /* Sum up all counts for this length (n = target) */
            seq_t sum = 0;
            for (i = 0; i < STATE_SIZE; i++)
                sum += old_dp_cur[i];
            ans[target] = sum;
        }
        dp_cur = old_dp_cur;
        ans_cap = new_cap;
    }

    return ans[n];
}
