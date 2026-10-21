/*
 * A241934  Number of length n+4 0..6 arrays with no consecutive five elements
 *          summing to more than 2*6.
 * https://oeis.org/A241934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 23, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241934
   a(n) = number of length (n+4) arrays of digits 0..6 with no
          consecutive 5 elements summing to more than 12.
   The algorithm uses a DP over the last 4 digits; it is O(n) in time
   and O(STATES) ≈ 2401 in memory. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
#  include <vector>
#endif

/* Parameters of the state machine */
#define STATES       2401          /* 7^4 */
#define DIGITS       4
#define MAX_VAL      6             /* digits 0..6 */
#define MAX_WINDOW_SUM 12          /* 5×6/2 = 12 */

/* Transition tables (initialized once) */
static int8_t   sum_of_state[STATES];          /* sum of the 4 digits of a state   */
static int16_t  transition[STATES][MAX_VAL+1]; /* next state or -1 if forbidden    */

static bool initialized = false;

/* Decode a state index into its 4 base‑7 digits */
static void decode_state(int idx, int *a, int *b, int *c, int *d) {
    *a = idx / 343;                /* 7^3 */
    *b = (idx / 49) % 7;           /* 7^2 */
    *c = (idx / 7) % 7;            /* 7^1 */
    *d = idx % 7;                  /* 7^0 */
}

/* Build the transition and sum tables */
static void init_transition(void) {
    for (int s = 0; s < STATES; ++s) {
        int a, b, c, d;
        decode_state(s, &a, &b, &c, &d);
        sum_of_state[s] = a + b + c + d;
        for (int e = 0; e <= MAX_VAL; ++e) {
            if ((int)sum_of_state[s] + e <= MAX_WINDOW_SUM) {
                int ns = ((b * 7 + c) * 7 + d) * 7 + e;
                transition[s][e] = (int16_t)ns;
            } else {
                transition[s][e] = -1;
            }
        }
    }
    initialized = true;
}

/* DP buffers */
#ifdef __cplusplus
static std::vector<seq_t> prev_counts;
static std::vector<seq_t> curr_counts;
static std::vector<seq_t> result_cache;
#else
static seq_t *prev_counts = NULL;
static seq_t *curr_counts = NULL;
static seq_t *result_cache = NULL;
#endif

/* Highest n value for which a(n) is cached (a(1) has index 0) */
static idx_t max_n_cached = 0;

/* Prepare the DP buffers and base case */
static void prepare_buffers(void) {
    if (!initialized) init_transition();

    /* Allocate buffers for DP */
#ifdef __cplusplus
    if (prev_counts.empty()) {
        prev_counts.assign(STATES, (seq_t)1);      /* length 4 (any first 4) */
        curr_counts.assign(STATES, (seq_t)0);
    }
    if (result_cache.empty()) {
        result_cache.resize(1);   /* will grow lazily */
    }
#else
    if (!prev_counts) {
        prev_counts = calloc(STATES, sizeof(seq_t));
        curr_counts = calloc(STATES, sizeof(seq_t));
        if (!prev_counts || !curr_counts) abort();
        for (int i = 0; i < STATES; ++i)
            prev_counts[i] = (seq_t)1;   /* base case length 4 */
    }
    if (!result_cache) {
        result_cache = calloc(1, sizeof(seq_t));
        if (!result_cache) abort();
    }
#endif
}

/* Ensure that a(n) is computed and stored for all n up to the requested one */
static void ensure_cache(idx_t n) {
    if (max_n_cached >= n) return;
    prepare_buffers();

    /* Grow result cache if necessary */
#ifdef __cplusplus
    if (result_cache.size() <= (size_t)n) result_cache.resize((size_t)n);
#else
    size_t new_size = (size_t)n;
    seq_t *tmp = realloc(result_cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    result_cache = tmp;
#endif

    /* Current DP length corresponds to max_n_cached+4 */
    for (idx_t cur_n = max_n_cached + 1; cur_n <= n; ++cur_n) {
        /* DP step: extend sequences by one new digit */
#ifdef __cplusplus
        std::fill(curr_counts.begin(), curr_counts.end(), (seq_t)0);
#else
        for (int i = 0; i < STATES; ++i) curr_counts[i] = (seq_t)0;
#endif
        for (int s = 0; s < STATES; ++s) {
            seq_t cnt = prev_counts[s];
            if (cnt == (seq_t)0) continue;
            for (int e = 0; e <= MAX_VAL; ++e) {
                int ns = (int)transition[s][e];
                if (ns >= 0) {
#ifdef __cplusplus
                    curr_counts[ns] += cnt;
#else
                    curr_counts[ns] = curr_counts[ns] + cnt;
#endif
                }
            }
        }

        /* Swap buffers */
#ifdef __cplusplus
        std::swap(prev_counts, curr_counts);
#else
        seq_t *tmp = prev_counts; prev_counts = curr_counts; curr_counts = tmp;
#endif

        /* Sum all counts to get a(cur_n) */
        seq_t total = (seq_t)0;
        for (int i = 0; i < STATES; ++i)
            total = total + prev_counts[i];
        result_cache[cur_n - 1] = total;   /* a(1) is at index 0 */
    }
    max_n_cached = n;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* n >= 1 by problem statement; undefined behaviour for n==0 */
    ensure_cache(n);
    return result_cache[n - 1];
}
