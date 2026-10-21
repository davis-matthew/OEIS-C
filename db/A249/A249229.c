/*
 * A249229  Number of length n+5 0..4 arrays with no six consecutive terms
 *          having two times the sum of any two elements equal to the sum of
 *          the remaining four.
 * https://oeis.org/A249229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 28, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249229: number of length n+5 arrays over {0..4}
   with no six consecutive terms such that
   2*(x_i + x_j) = sum of the remaining four.  
   Computed by a deterministic finite automaton with 5⁵ states. */

#include <stdlib.h>
#include <stdbool.h>

/* constants */
#define NUM_STATES 3125          /* 5^5 */
#define ALPHABET   5
#define POW5_4     625            /* 5^4 */

/* transition tables */
static uint8_t  allowed[NUM_STATES][ALPHABET];
static uint16_t next_state[NUM_STATES][ALPHABET];  /* fits in 16 bits */

/* sequence storage */
#define MAX_N        20000          /* sufficient for n up to 10 000 */
static seq_t answers[MAX_N + 1] = {0};   /* zero‑initialised for both C and C++ */
static seq_t cur[NUM_STATES] = {0};
static seq_t nxt[NUM_STATES] = {0};

/* state tracking */
static bool trans_inited  = false;
static idx_t max_n        = 0;      /* the largest n for which answers[] is valid */

/* initialise transition tables once */
static void init_transitions(void)
{
    if (trans_inited) return;
    for (uint16_t s = 0; s < NUM_STATES; ++s) {
        int digit[5];
        uint16_t tmp = s;
        for (int i = 0; i < 5; ++i) {
            digit[i] = tmp % 5;
            tmp     /= 5;
        }
        for (uint16_t v = 0; v < ALPHABET; ++v) {
            int t[6];
            for (int i = 0; i < 5; ++i) t[i] = digit[i];
            t[5] = (int)v;
            int sum = 0;
            for (int i = 0; i < 6; ++i) sum += t[i];
            bool bad = false;
            for (int i = 0; i < 6 && !bad; ++i)
                for (int j = i + 1; j < 6; ++j)
                    if (3 * (t[i] + t[j]) == sum) { bad = true; break; }
            allowed[s][v] = !bad;
            next_state[s][v] = (s / 5) + v * POW5_4;
        }
    }
    trans_inited = true;

    /* initialise DP for length 5: each suffix counts as one */
    for (uint16_t s = 0; s < NUM_STATES; ++s) cur[s] = 1;
    max_n = 0;   /* no full arrays yet */
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;   /* undefined for n<1, return 0 safely */

    /* ensure tables are ready */
    init_transitions();

    /* extend precomputation up to required n */
    while (max_n < n) {
        /* zero next buffer */
        for (uint16_t s = 0; s < NUM_STATES; ++s) nxt[s] = 0;

        /* DP transition */
        for (uint16_t s = 0; s < NUM_STATES; ++s) {
            seq_t cnt = cur[s];
            if (!cnt) continue;
            for (uint16_t v = 0; v < ALPHABET; ++v) {
                if (!allowed[s][v]) continue;
                uint16_t ns = next_state[s][v];
                nxt[ns] = nxt[ns] + cnt;
            }
        }

        /* swap buffers for next length */
        for (uint16_t s = 0; s < NUM_STATES; ++s) cur[s] = nxt[s];

        /* sum all states to obtain a(max_n+1) */
        seq_t sum = 0;
        for (uint16_t s = 0; s < NUM_STATES; ++s) sum = sum + cur[s];

        ++max_n;
        answers[max_n] = sum;
    }

    /* return the requested term; answers[0] is unused */
    return answers[(idx_t)n];
}
