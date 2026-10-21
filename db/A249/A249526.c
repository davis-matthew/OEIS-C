/*
 * A249526  Number of length n+5 0..4 arrays with no six consecutive terms
 *          having five times any element equal to the sum of the remaining
 *          five.
 * https://oeis.org/A249526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) – number of arrays of length n+5 over {0,1,2,3,4} that
 *            contain no length‑6 subarray in which an element equals (sum of
 *            the other five)/5. The algorithm builds an automaton on the last
 *            five digits, precomputes all allowed transitions, then performs
 *            a simple DP for n steps
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 24, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define STATES 3125        /* 5^5 possible last–five tuples */
#define ALPHA  5           /* alphabet size */

/* Precomputed data */
static uint8_t  digit_map[STATES][5];  /* last five digits of a state */
static uint16_t next_state[STATES][ALPHA]; /* transition state id  */
static uint8_t  forbid_map[STATES][ALPHA];  /* 1 if the transition is forbidden */
static bool initialized = false;

/* Helper to compute state id from the five digits (little‑endian). */
static inline uint16_t encode_state(const uint8_t d[5]) {
    return d[0] + 5 * (d[1] + 5 * (d[2] + 5 * (d[3] + 5 * d[4])));
}

/* Pre‑compute transitions and forbidden flags.  */
static void precompute(void) {
    if (initialized) return;
    /* enumerate all states: base‑5 numbers of length 5 */
    for (int s = 0; s < STATES; ++s) {
        uint8_t d[5];
        int tmp = s;
        for (int i = 0; i < 5; ++i) { d[i] = tmp % ALPHA; tmp /= ALPHA; }
        digit_map[s][0] = d[0];
        digit_map[s][1] = d[1];
        digit_map[s][2] = d[2];
        digit_map[s][3] = d[3];
        digit_map[s][4] = d[4];

        /* count occurrences of each digit within this state */
        uint8_t cnt[ALPHA] = {0};
        int sum5 = 0;
        for (int i = 0; i < 5; ++i) {
            cnt[d[i]]++;
            sum5 += d[i];
        }

        for (int x = 0; x < ALPHA; ++x) {
            int sum = sum5 + x;                         /* total sum of six digits */
            int forbidden = 0;
            if (sum % 6 == 0) {
                int avg = sum / 6;
                if (avg >= 0 && avg < ALPHA)                 /* avg in {0..4} */
                    if (cnt[avg] > 0 || x == avg) forbidden = 1;
            }
            forbid_map[s][x] = forbidden;
            if (!forbidden) {
                /* form new state by dropping the oldest digit and adding x */
                uint8_t nd[5] = { d[1], d[2], d[3], d[4], (uint8_t)x };
                next_state[s][x] = encode_state(nd);
            } else {
                next_state[s][x] = 0; /* placeholder, not used */
            }
        }
    }
    initialized = true;
}

/* Main function required by the judge.  */
seq_t a(idx_t n) {
    precompute();

    /* static DP arrays reused for each call */
    static seq_t dp[STATES];
    static seq_t ndp[STATES];

    /* initialise dp to 1 for all starting five‑digit states */
    for (size_t i = 0; i < STATES; ++i) dp[i] = 1;

    /* perform n transitions (length n+5 arrays) */
    for (idx_t step = 0; step < n; ++step) {
        /* zero the new DP array */
        for (size_t i = 0; i < STATES; ++i) ndp[i] = 0;

        for (size_t s = 0; s < STATES; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;          /* skip empty counts */

            for (int x = 0; x < ALPHA; ++x) {
                if (forbid_map[s][x]) continue;
                size_t ns = next_state[s][x];
                ndp[ns] += val;
            }
        }
        /* swap dp and ndp for next step */
        for (size_t i = 0; i < STATES; ++i) dp[i] = ndp[i];
    }

    /* sum all sequences after n steps */
    seq_t total = 0;
    for (size_t i = 0; i < STATES; ++i) total += dp[i];
    return total;
}
