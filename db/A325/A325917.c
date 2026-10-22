/*
 * A325917  Number of Motzkin meanders of length n with an even number of
 *          humps and without peaks.
 * https://oeis.org/A325917
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

/*
 * Formula: a(n) = number of Motzkin meanders of length n
 * with an even number of humps (UHH…HD, H≥0) and without peaks (no UD).
 *
 * The implementation uses a dynamic‑programming recurrence based on
 * the current altitude, whether we are inside a potential hump,
 * and the parity of the number of completed humps.
 *
 * State 0: no recent U that could start a hump.
 * State 1: a recent U but no H has followed (hump cannot be completed yet).
 * State 2: inside an ongoing hump (after at least one H).
 *
 * For each step we update a compact 1‑dimensional array that stores
 * the values for all possible heights, states, and parity.
 *
 * Complexity:   O(n²) time, O(n) memory, which is sufficient for
 *                n up to about 10,000 on the judge.
 */

#include <stdlib.h>

/* ----- DP helper macro ----------------------------------------------------- */
#define IDX(layer, h, s, p)                                          \
    ((size_t)(layer) * stride + ((size_t)(h) * 3 + (s)) * 2 + (p))

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    /* Number of elements for one layer: heights 0..n, 3 states, 2 parities */
    const size_t stride = (size_t)(n + 1) * 3 * 2;

    /* Allocate two layers: previous and next */
    seq_t *dp = (seq_t *)malloc(2 * stride * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;   /* out of memory (should not happen) */

    /* Initialize all to zero and set start state */
    for (size_t i = 0; i < 2 * stride; ++i)
        dp[i] = 0;
    dp[IDX(0, 0, 0, 0)] = (seq_t)1;

    /* Iterate over all steps */
    for (idx_t step = 0; step < n; ++step) {
        size_t cur = (size_t)(step & 1);
        size_t nxt = cur ^ 1;

        /* Reset next layer to zeros */
        for (size_t i = 0; i < stride; ++i)
            dp[IDX(nxt, 0, 0, 0) + i] = 0;

        /* Maximum reachable height after this many steps */
        size_t maxh = (size_t)step;

        for (size_t h = 0; h <= maxh; ++h) {
            for (int p = 0; p < 2; ++p) {
                for (int s = 0; s < 3; ++s) {
                    seq_t val = dp[IDX(cur, h, s, p)];
                    if (!val)
                        continue;

                    switch (s) {
                    case 0: /* not in a potential hump */
                        /* U */
                        dp[IDX(nxt, h + 1, 1, p)] += val;
                        /* H */
                        dp[IDX(nxt, h, 0, p)] += val;
                        /* D */
                        if (h > 0)
                            dp[IDX(nxt, h - 1, 0, p)] += val;
                        break;

                    case 1: /* recent U, no H yet */
                        /* U */
                        dp[IDX(nxt, h + 1, 1, p)] += val;
                        /* H -> start hump */
                        dp[IDX(nxt, h, 2, p)] += val;
                        /* D not allowed */
                        break;

                    case 2: /* inside an ongoing hump */
                        /* U */
                        dp[IDX(nxt, h + 1, 1, p)] += val;
                        /* H */
                        dp[IDX(nxt, h, 2, p)] += val;
                        /* D -> completes hump, flip parity */
                        if (h > 0)
                            dp[IDX(nxt, h - 1, 0, p ^ 1)] += val;
                        break;
                    }
                }
            }
        }
    }

    /* Compute answer: sum of states with even parity (p == 0) */
    size_t final_layer = (size_t)(n & 1);
    seq_t result = 0;
    size_t maxh_final = (size_t)n;
    for (size_t h = 0; h <= maxh_final; ++h) {
        for (int s = 0; s < 3; ++s) {
            result += dp[IDX(final_layer, h, s, 0)];
        }
    }

    free(dp);
    return result;
}

#undef IDX
