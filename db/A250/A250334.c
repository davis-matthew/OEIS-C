/*
 * A250334  Number of length n+5 0..6 arrays with every six consecutive terms
 *          having the maximum of some three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 0..6 arrays in which every *
 *            consecutive six terms satisfy the condition that the maximum *
 *            of some three terms equals the minimum of the remaining three. *
 *            Computation is done by dynamic programming over the last five *
 *            values (states)
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 26, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We rely on the harness to define seq_t (value type) and idx_t
 * (index type).  No other typedefs are made. */

#include <stdbool.h>
#include <stdint.h>       /* for uint8_t */

#define ALPH 7                 /* alphabet 0..6 */
#define LSTATE 5               /* length of state (last 5 values) */
#define MAXS (16807)           /* ALPH^LSTATE == 7^5 */

/* Transition table: for each state (encoded as 5‐tuple) list all
 * successor states reachable by adding a new element that satisfies
 * the 6‑window condition.  Each state has at most ALPH successors. */
static int trans[MAXS][ALPH];
static unsigned char trans_cnt[MAXS];
static uint8_t stvals[MAXS][LSTATE];
static bool initialized = false;

/* ------------------------------------------------------------ */
/* Build transition table once                                            */
static void build_states(void)
{
    if (initialized) {
        return;
    }
    initialized = true;

    /* Decode each state into its 5‐tuple */
    for (int s = 0; s < MAXS; ++s) {
        int tmp = s;
        for (int i = LSTATE - 1; i >= 0; --i) {
            stvals[s][i] = (uint8_t)(tmp % ALPH);
            tmp /= ALPH;
        }
    }

    /* Precompute transitions */
    const int pow7_4 = 2401;          /* 7^4 */
    for (int s = 0; s < MAXS; ++s) {
        trans_cnt[s] = 0;
        /* Current 5 values */
        uint8_t a0 = stvals[s][0];
        uint8_t a1 = stvals[s][1];
        uint8_t a2 = stvals[s][2];
        uint8_t a3 = stvals[s][3];
        uint8_t a4 = stvals[s][4];

        int base = s;                /* for new_state computation */

        for (int x = 0; x < ALPH; ++x) {
            /* Form 6 values and check condition */
            uint8_t arr[6] = { a0, a1, a2, a3, a4, (uint8_t)x };
            /* Simple insertion sort of 6 elements */
            for (int i = 1; i < 6; ++i) {
                uint8_t key = arr[i];
                int j = i - 1;
                while (j >= 0 && arr[j] > key) {
                    arr[j + 1] = arr[j];
                    --j;
                }
                arr[j + 1] = key;
            }
            if (arr[2] == arr[3]) {          /* window satisfied */
                int new_state = ((base % pow7_4) * ALPH) + x;
                trans[s][trans_cnt[s]++] = new_state;
            }
        }
    }
}
/* ------------------------------------------------------------ */

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is the offset (>=1).  The total length of the array is n+5,
     * so we must perform n transitions (adding n new elements). */
    idx_t steps = n;

    build_states();               /* build transitions if not done */

    /* Allocate DP arrays */
    seq_t *dp_cur;
    seq_t *dp_next;

#ifdef __cplusplus
    dp_cur = new seq_t[MAXS];
    dp_next = new seq_t[MAXS];
#else
    dp_cur = (seq_t *)malloc(MAXS * sizeof(seq_t));
    dp_next = (seq_t *)malloc(MAXS * sizeof(seq_t));
#endif

    /* Initial vector: any 5‑tuple is a valid start (count 1) */
    for (int i = 0; i < MAXS; ++i) {
        dp_cur[i] = (seq_t)1;
    }

    /* Perform DP steps */
    for (idx_t s = 0; s < steps; ++s) {
        /* zero dp_next */
        for (int i = 0; i < MAXS; ++i) {
            dp_next[i] = (seq_t)0;
        }

        /* transitions */
        for (int state = 0; state < MAXS; ++state) {
            seq_t cur_val = dp_cur[state];
            if (cur_val == 0) {
                continue;
            }
            unsigned char m = trans_cnt[state];
            for (unsigned char j = 0; j < m; ++j) {
                int ns = trans[state][j];
                dp_next[ns] += cur_val;
            }
        }

        /* swap dp_cur and dp_next */
        seq_t *tmp = dp_cur;
        dp_cur = dp_next;
        dp_next = tmp;
    }

    /* Sum final vector to obtain answer */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < MAXS; ++i) {
        ans += dp_cur[i];
    }

    /* free memory */
#ifdef __cplusplus
    delete[] dp_cur;
    delete[] dp_next;
#else
    free(dp_cur);
    free(dp_next);
#endif

    return ans;
}
