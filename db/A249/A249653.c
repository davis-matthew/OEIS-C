/*
 * A249653  Number of length n+4 0..5 arrays with every five consecutive terms
 *          having four times some element equal to the sum of the remaining
 *          four.
 * https://oeis.org/A249653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 arrays over {0,…,5} satisfying:
 *            every 5 consecutive terms have four times some element equal to
 *            the sum of the remaining four
 * Range:     u8 none, u16 n <= 21, u32 n <= 61, u64 n <= 137, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 17‑state‐based dynamic programming: each state keeps the last
   four digits.  Transitions are checked for the 5‑tuple condition. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STATES (6*6*6*6)   /* 1296 four‑digit states */

/* Pre‑computed transition table */
static int16_t nextstate[STATES][6];     /* -1 = forbidden   */
static uint8_t digits[STATES][4];        /* decoded digits   */

/* DP buffers for a given length */
static seq_t *dp_curr = NULL, *dp_next = NULL;

/* Cached answers */
static seq_t *answers = NULL;
static idx_t ans_size = 0;    /* last computed index (n) */
static size_t ans_cap = 0;

/* Current length of sequences represented by dp_curr (initially 4) */
static idx_t current_len = 4;

/* One-time initializer */
static void init_sequence(void)
{
    /* Decode digits and compute all admissible transitions */
    for (idx_t s = 0; s < STATES; ++s) {
        idx_t tmp = s;
        digits[s][3] = tmp % 6; tmp /= 6;
        digits[s][2] = tmp % 6; tmp /= 6;
        digits[s][1] = tmp % 6; tmp /= 6;
        digits[s][0] = tmp % 6;

        for (int x = 0; x < 6; ++x) {
            int sum = digits[s][0] + digits[s][1] + digits[s][2] + digits[s][3] + x;
            if (sum % 5 == 0) {
                int avg = sum / 5;
                if (avg == digits[s][0] || avg == digits[s][1] ||
                    avg == digits[s][2] || avg == digits[s][3] || avg == x) {
                    int ns = ((((digits[s][1] * 6) + digits[s][2]) * 6 + digits[s][3]) * 6 + x);
                    nextstate[s][x] = (int16_t)ns;
                } else {
                    nextstate[s][x] = -1;
                }
            } else {
                nextstate[s][x] = -1;
            }
        }
    }

    /* Allocate DP buffers */
    dp_curr = (seq_t *)malloc(STATES * sizeof(seq_t));
    dp_next = (seq_t *)malloc(STATES * sizeof(seq_t));
    for (idx_t s = 0; s < STATES; ++s)
        dp_curr[s] = (seq_t)1;    /* one sequence for each 4‑digit state */

    /* Allocate answer cache */
    ans_cap = 64;                 /* initial capacity */
    answers = (seq_t *)malloc((ans_cap + 1) * sizeof(seq_t));
    memset(answers, 0, (ans_cap + 1) * sizeof(seq_t));
    ans_size = 0;
}

/* Ensure answers are known up to index n (inclusive) */
static void ensure_answers(idx_t n)
{
    if (ans_size >= n)
        return;

    /* Grow answer array if necessary */
    if (ans_cap <= n) {
        size_t new_cap = n * 2 + 64;
        seq_t *new_arr = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
        memcpy(new_arr, answers, (ans_cap + 1) * sizeof(seq_t));
        free(answers);
        answers = new_arr;
        ans_cap = new_cap;
    }

    /* Extend DP until we have sequences of length n+4 */
    while (current_len < n + 4) {
        memset(dp_next, 0, STATES * sizeof(seq_t));

        for (idx_t s = 0; s < STATES; ++s) {
            seq_t val = dp_curr[s];
            if (!val)
                continue;
            for (int x = 0; x < 6; ++x) {
                int16_t t = nextstate[s][x];
                if (t < 0)
                    continue;
                dp_next[t] += val;
            }
        }

        /* new_n corresponds to length (current_len+1) - 4 */
        idx_t new_n = current_len + 1 - 4;
        seq_t total = 0;
        for (idx_t s = 0; s < STATES; ++s)
            total += dp_next[s];
        answers[new_n] = total;

        /* Prepare for next iteration */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;
        ++current_len;
    }

    ans_size = n;
}

/* Public entry function */
seq_t a(idx_t n)
{
    if (!dp_curr)          /* first call */
        init_sequence();
    ensure_answers(n);
    return answers[n];
}
