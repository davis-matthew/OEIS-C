/*
 * A249194  Number of length n+6 0..5 arrays with no seven consecutive terms
 *          having four times the sum of any three elements equal to three
 *          times the sum of the remaining four.
 * https://oeis.org/A249194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+6 arrays over 0..5 with no seven *
 *            consecutive terms having four times the sum of any three
 *            elements * equal to three times the sum of the remaining four
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 20, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define STATE_COUNT 46656  /* 6^6 */
#define POWER6_5   7776    /* 6^5 */

/* Pre‑computed transition tables */
static uint32_t (*next_state)[6] = NULL;
static uint8_t  *out_count      = NULL;
static bool     transitions_computed = false;

/* DP vectors */
static seq_t *cur_vec  = NULL;
static seq_t *next_vec = NULL;

/* Cached results */
static seq_t *results     = NULL;
static size_t results_cap = 0;
static idx_t  max_n_done  = 0;

/* ------------------------------------------------------------------ */

static void init_transitions(void)
{
    if (transitions_computed) return;

    /* Allocate transition tables */
    next_state = (uint32_t (*)[6])malloc(sizeof(uint32_t) * STATE_COUNT * 6);
    out_count  = (uint8_t *)malloc(sizeof(uint8_t) * STATE_COUNT);
    if (!next_state || !out_count) abort();

    for (int s = 0; s < STATE_COUNT; ++s) {
        int temp = s;
        int digits[6];
        int sum6 = 0;

        /* Extract the 6 digits of the state and compute their sum */
        for (int d = 0; d < 6; ++d) {
            digits[d] = temp % 6;
            temp      /= 6;
            sum6      += digits[d];
        }

        out_count[s] = 0;

        /* Try appending each of the 6 possible digits */
        for (int x = 0; x < 6; ++x) {
            int sum7 = sum6 + x;
            bool forbidden = false;
            int arr[7];
            for (int i = 0; i < 6; ++i) arr[i] = digits[i];
            arr[6] = x;

            /* Check the forbidden condition for all triplets */
            for (int a = 0; a < 5 && !forbidden; ++a)
                for (int b = a + 1; b < 6 && !forbidden; ++b)
                    for (int c = b + 1; c < 7; ++c) {
                        int sum3 = arr[a] + arr[b] + arr[c];
                        if (7 * sum3 == 3 * sum7) {
                            forbidden = true;
                            break;
                        }
                    }

            if (!forbidden) {
                uint32_t ns = (uint32_t)((s % POWER6_5) * 6 + x);
                next_state[s][out_count[s]++] = ns;
            }
        }
    }

    transitions_computed = true;
}

/* ------------------------------------------------------------------ */

static void init_once(void)
{
    if (cur_vec) return;          /* already initialized */

    init_transitions();

    cur_vec  = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
    next_vec = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
    if (!cur_vec || !next_vec) abort();

    /* Every 6‑tuple is a valid initial state */
    for (int i = 0; i < STATE_COUNT; ++i) cur_vec[i] = 1;

    /* Results array (index 0 unused for clarity) */
    results = (seq_t *)malloc(sizeof(seq_t) * 1);
    if (!results) abort();
    results[0] = 0;
    results_cap = 1;
    max_n_done = 0;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    init_once();

    /* Resize results buffer if necessary */
    if ((size_t)(n + 1) > results_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        results = tmp;
        results_cap = new_cap;
    }

    /* Compute the dynamic programming up to the required n */
    for (idx_t step = max_n_done + 1; step <= n; ++step) {
        /* Zero the next vector */
        memset(next_vec, 0, sizeof(seq_t) * STATE_COUNT);

        /* Apply all transitions */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur_vec[s];
            if (!val) continue;
            uint8_t oc = out_count[s];
            uint32_t *tgt = next_state[s];
            for (uint8_t t = 0; t < oc; ++t) {
                next_vec[tgt[t]] += val;
            }
        }

        /* Swap current and next vectors */
        seq_t *tmp = cur_vec;
        cur_vec = next_vec;
        next_vec = tmp;

        /* Sum all counts for this length */
        seq_t total = 0;
        for (int s = 0; s < STATE_COUNT; ++s) total += cur_vec[s];
        results[step] = total;
    }

    max_n_done = n;
    return results[n];
}
