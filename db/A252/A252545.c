/*
 * A252545  Number of (1+2)X(n+2) 0..3 arrays with every 3X3 subblock row and
 *          diagonal sum equal to 1 3 4 6 or 7 and every 3X3 column and
 *          antidiagonal sum not equal to 1 3 4 6 or 7.
 * https://oeis.org/A252545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count 3 × (n+2) arrays of digits 0..3 such that for every 3×3
 *            subblock the sums of its rows and its main diagonal are one of
 *            {1,3,4,6,7}, whereas the sums of its columns and its
 *            anti‑diagonal are NOT in that set. The function returns the
 *            exact count in seq_t
 * Range:     u8 none, u16 n <= 6, u32 n <= 18, u64 n <= 42, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define STATES (64)              /* column states: 3 digits each 0..3  */
#define PAIRS  (STATES*STATES)   /* pair of consecutive columns   */

static bool allowed_sum[10];
static uint8_t state_digit[STATES][3];
static uint8_t state_colsum[STATES];
static uint64_t trans_mask[PAIRS];

/* One‑time initialization: allowed sums, state encodings, and
   transition masks for all 64×64 (a,b) pairs. */
static void init(void)
{
    /* Allowed sums set {1,3,4,6,7} */
    for (int i = 0; i < 10; ++i) allowed_sum[i] = false;
    allowed_sum[1] = allowed_sum[3] = allowed_sum[4] = allowed_sum[6] = allowed_sum[7] = true;

    /* Pre‑compute digits of each column state and its column sum. */
    for (int s = 0; s < STATES; ++s) {
        state_digit[s][0] = (s >> 4) & 3;
        state_digit[s][1] = (s >> 2) & 3;
        state_digit[s][2] = s & 3;
        state_colsum[s] = state_digit[s][0] + state_digit[s][1] + state_digit[s][2];
    }

    /* Build transition masks: for each pair (a,b) the set of c that
       make (a,b,c) satisfy the subblock constraints. */
    for (int a = 0; a < STATES; ++a) {
        for (int b = 0; b < STATES; ++b) {
            uint64_t mask = 0;
            for (int c = 0; c < STATES; ++c) {
                /* Row sums */
                int r0 = state_digit[a][0] + state_digit[b][0] + state_digit[c][0];
                if (!allowed_sum[r0]) continue;
                int r1 = state_digit[a][1] + state_digit[b][1] + state_digit[c][1];
                if (!allowed_sum[r1]) continue;
                int r2 = state_digit[a][2] + state_digit[b][2] + state_digit[c][2];
                if (!allowed_sum[r2]) continue;
                /* Column sums not allowed */
                if (allowed_sum[state_colsum[a]]) continue;
                if (allowed_sum[state_colsum[b]]) continue;
                if (allowed_sum[state_colsum[c]]) continue;
                /* Main diagonal sum must be allowed */
                int d_main = state_digit[a][0] + state_digit[b][1] + state_digit[c][2];
                if (!allowed_sum[d_main]) continue;
                /* Anti diagonal sum must NOT be allowed */
                int d_anti = state_digit[a][2] + state_digit[b][1] + state_digit[c][0];
                if (allowed_sum[d_anti]) continue;
                mask |= (uint64_t)1 << c;
            }
            trans_mask[a * STATES + b] = mask;
        }
    }
}

/* Sequence evaluation.  memoises all values up to the greatest n
   requested so far.  The array length is n+2 columns; we count the
   number of valid fills. */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *dp_prev = NULL, *dp_curr = NULL;
    static seq_t *a_vals  = NULL;
    static size_t a_cap   = 0;
    static idx_t steps_done = 0;

    /* Initialize on first call. */
    if (!initialized) {
        init();
        dp_prev = (seq_t*)calloc(PAIRS, sizeof(seq_t));
        dp_curr = (seq_t*)calloc(PAIRS, sizeof(seq_t));
        for (size_t i = 0; i < PAIRS; ++i) dp_prev[i] = 1;   /* all 2‑column prefixes */
        a_cap   = 16;
        a_vals  = (seq_t*)calloc(a_cap, sizeof(seq_t));
        initialized = true;
    }

    /* Grow a_vals array if needed. */
    if (n >= a_cap) {
        size_t new_cap = a_cap * 2;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        for (size_t i = a_cap; i < new_cap; ++i) tmp[i] = 0;
        a_vals = tmp;
        a_cap  = new_cap;
    }

    /* Advance DP up to the required n. */
    while (steps_done < n) {
        memset(dp_curr, 0, PAIRS * sizeof(seq_t));

        for (size_t idx = 0; idx < PAIRS; ++idx) {
            seq_t val = dp_prev[idx];
            if (!val) continue;

            uint64_t mask = trans_mask[idx];
            while (mask) {
                int r = __builtin_ctzll(mask);               /* next allowed column */
                dp_curr[(idx % STATES) * STATES + r] += val;
                mask &= mask - 1;                            /* clear lowest set bit */
            }
        }

        /* Sum all counts for arrays of length steps_done+2 columns. */
        seq_t total = 0;
        for (size_t i = 0; i < PAIRS; ++i) total += dp_curr[i];
        a_vals[steps_done + 1] = total;

        /* Prepare for next iteration. */
        seq_t *tmp = dp_prev;
        dp_prev   = dp_curr;
        dp_curr   = tmp;
        ++steps_done;
    }

    return a_vals[n];
}
