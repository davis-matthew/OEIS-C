/*
 * A262845  Number of (n+2)X(3+2) 0..1 arrays with each row divisible by 7 and
 *          column not divisible by 7, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A262845: Number of (n+2)×5 binary arrays with each row divisible by 7
 * and each column not divisible by 7 (read top‑left as most significant bits).
 * This implementation uses dynamic programming over the 5‑dimensional state
 * space of column remainders modulo 7.  The state transitions are pre‑computed
 * once and the DP is expanded incrementally on demand, so arbitrary‑precision
 * arithmetic is handled correctly for all built configurations.
 */
#include <stdint.h>
#include <stdbool.h>

#define STATE_COUNT 16807u   /* 7^5 states */

/* forward declarations of seq_t functions are assumed to exist in the build */

/* pre‑computed transition table: trans[s][p] = next state after applying pattern p in state s */
static unsigned short trans[STATE_COUNT][5];
/* flag indicating whether all column remainders in a state are non‑zero */
static bool valid_state[STATE_COUNT];

/* DP arrays – two buffers that swap after each row */
static seq_t dp_arr1[STATE_COUNT];
static seq_t dp_arr2[STATE_COUNT];
static seq_t *dp_arr;   /* points to current DP buffer (rows processed so far) */
static seq_t *next_arr; /* points to buffer used for the next state */

/* cache of computed sequence values */
#define CACHE_MAX 20005
static seq_t cache[CACHE_MAX];
static idx_t cache_max_n = 0;     /* largest n for which cache[n] is valid */

/* current number of rows processed (corresponding to dp_arr state) */
static idx_t cur_rows = 0;

/* initialisation flag */
static bool initialized = false;

/* --------------------------------------------------------------------- */
/* Helper functions: initialise transition table and DP buffer         */
/* --------------------------------------------------------------------- */
static void init_once(void)
{
    /* patterns in decimal: 0 => 00000, 7 => 00111, 14 => 01110, 21 => 10101, 28 => 11100 */
    const int patterns[5] = {0, 7, 14, 21, 28};

    /* pre‑compute transition table and valid_state flags */
    for (int s = 0; s < STATE_COUNT; ++s) {
        int t = s;
        int rem[5];
        bool any_zero = false;
        /* decode 5 base‑7 digits: remainder for each of the 5 columns */
        for (int i = 4; i >= 0; --i) {
            rem[i] = t % 7;
            t /= 7;
            if (rem[i] == 0) any_zero = true;
        }
        valid_state[s] = !any_zero;

        for (int p = 0; p < 5; ++p) {
            int new_rem[5];
            for (int i = 0; i < 5; ++i) {
                int bit = (patterns[p] >> (4 - i)) & 1;
                new_rem[i] = (rem[i] * 2 + bit) % 7;
            }
            int ns = 0;
            for (int i = 0; i < 5; ++i) {
                ns = ns * 7 + new_rem[i];
            }
            trans[s][p] = (unsigned short)ns;
        }
    }

    /* initialise DP buffers */
    dp_arr = dp_arr1;
    next_arr = dp_arr2;
    for (int i = 0; i < STATE_COUNT; ++i) dp_arr[i] = 0;
    dp_arr[0] = (seq_t)1;
    cur_rows = 0;
    cache_max_n = 0;
    initialized = true;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init_once();

    if (n <= cache_max_n) return cache[n];

    /* target number of rows to process: n+2 */
    idx_t target_rows = n + 2;

    for (idx_t r = cur_rows + 1; r <= target_rows; ++r) {
        /* zero next_arr */
        for (int i = 0; i < STATE_COUNT; ++i) next_arr[i] = 0;

        /* transition from current DP */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_arr[s];
            if (val == 0) continue;
            for (int p = 0; p < 5; ++p) {
                unsigned short ns = trans[s][p];
                next_arr[ns] += val;
            }
        }

        /* swap dp and next buffers */
        seq_t *tmp = dp_arr;
        dp_arr = next_arr;
        next_arr = tmp;
        cur_rows = r;

        /* if a new (n') value becomes available, compute and cache it */
        if (cur_rows >= 2) {
            idx_t nprime = cur_rows - 2;
            if (nprime > cache_max_n) {
                seq_t sum = 0;
                for (int s = 0; s < STATE_COUNT; ++s) {
                    if (valid_state[s]) sum += dp_arr[s];
                }
                if (nprime < CACHE_MAX) {
                    cache[nprime] = sum;
                    cache_max_n = nprime;
                }
            }
        }
    }

    return cache[n];
}
