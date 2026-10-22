/*
 * A365005  Number of ways to write 2 as a nonnegative linear combination of a
 *          strict integer partition of n.
 * https://oeis.org/A365005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   alloc_state, free_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = #strict partitions of n that contain part 1 but not 2
 *          + #strict partitions that contain part 2 but not 1
 *          + 2 * #strict partitions that contain both 1 and 2
 *
 * The value is computed by dynamic programming over distinct parts.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_state.h"
#include "../utils/free_state.h"

static seq_t *state[4] = {0, 0, 0, 0};
static idx_t max_computed = 0;

/* Recompute all four state tables up to new_max (inclusive). */
static void recompute(idx_t new_max)
{
    /* free old tables */
    for (int i = 0; i < 4; ++i) {
        if (state[i]) {
            free_state(state[i]);
            state[i] = 0;
        }
    }

    size_t len = (size_t)(new_max + 1);

    /* allocate new tables */
    for (int i = 0; i < 4; ++i)
        state[i] = alloc_state(len);

    /* base case: empty partition */
#ifdef __cplusplus
    state[0][0] = seq_t(1);
#else
    state[0][0] = (seq_t)1;
#endif

    /* dynamic programming for distinct parts */
    for (idx_t part = 1; part <= new_max; ++part) {
        for (idx_t sum = new_max; sum >= part; --sum) {
            for (int old = 0; old < 4; ++old) {
                seq_t val = state[old][sum - part];
                if (!val) continue;
                int newstate = old;
                if (part == 1) newstate |= 1;  /* include part 1 */
                if (part == 2) newstate |= 2;  /* include part 2 */
                state[newstate][sum] += val;
            }
        }
    }

    max_computed = new_max;
}

/* public function required by the evaluation harness */
seq_t a(idx_t n)
{
    if (n > max_computed) recompute(n);
    if (max_computed == 0) recompute(0);  /* first call with n==0 */

    seq_t A = state[1][n];
    seq_t B = state[2][n];
    seq_t C = state[3][n];

    return A + B + C + C;   /* A + B + 2*C */
}
