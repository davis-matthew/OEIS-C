/*
 * A246477  Number of length n+3 0..6 arrays with no pair in any consecutive
 *          four terms totaling exactly 6.
 * https://oeis.org/A246477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 25, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   A246477: Number of length n+3 arrays over {0..6} with no pair
    in any consecutive four terms totaling exactly 6.
    Computation is done by dynamic programming over the last 3 terms.
    For n>=1  (offset 1) the array has length n+3.
    The algorithm is O(n) with a very small constant
    (343 states × 7 transitions per state). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* number of possible states for the last three digits: 7^3 */
#define STATES 343
#define DIGITS 7

/* decode a state index into its three digits */
static inline void decode_state(idx_t idx, uint8_t *a, uint8_t *b, uint8_t *c)
{
    *a = idx / (DIGITS * DIGITS);
    idx %= DIGITS * DIGITS;
    *b = idx / DIGITS;
    *c = idx % DIGITS;
}

/* check that no pair among a,b,c,d sums to 6 */
static inline bool valid_triplet(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return !((a + b == 6) || (a + c == 6) || (a + d == 6) ||
             (b + c == 6) || (b + d == 6) || (c + d == 6));
}

/* DP tables (static so they survive between calls) */
static seq_t dp_cur[STATES];
static seq_t dp_next[STATES];

/* Main entry function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; undefined for 0 */

    /* initialise the base state: all 343 triples are possible for length 3 */
    for (idx_t i = 0; i < STATES; ++i) dp_cur[i] = (seq_t)1;

    /* each step adds one new element, so after n steps we have length 3+n */
    for (idx_t step = 0; step < n; ++step)
    {
        /* clear next table */
        for (idx_t i = 0; i < STATES; ++i) dp_next[i] = (seq_t)0;

        /* transition from every reachable state */
        for (idx_t idx = 0; idx < STATES; ++idx)
        {
            seq_t cnt = dp_cur[idx];
            if (cnt == 0) continue;

            uint8_t a0, b0, c0;
            decode_state(idx, &a0, &b0, &c0);

            for (uint8_t d = 0; d < DIGITS; ++d)
            {
                if (!valid_triplet(a0, b0, c0, d)) continue;

                idx_t newidx = b0 * (DIGITS * DIGITS) + c0 * DIGITS + d;
                dp_next[newidx] += cnt;
            }
        }

        /* swap tables for next iteration */
        for (idx_t i = 0; i < STATES; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all counts for the final length (3+n) */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < STATES; ++i)
        total += dp_cur[i];

    return total;
}
