/*
 * A247722  Number of length n+3 0..4 arrays with no disjoint pairs in any
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of length n+3 0..4 arrays with no disjoint pairs in any
 * consecutive four terms having the same sum.
 * Offset: 1 (a(1) is the first term listed in OEIS).
 *
 * Implementation uses dynamic programming over the last three digits.
 * For each length L ≥ 3 we store the number of arrays ending with each
 * triple (x1,x2,x3) that satisfy the property for all windows ending at
 * that triple.  Adding one more digit extends the window and we update
 * the counts.  The total number of arrays of length L is stored as a(L-3).
 */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- constants ----------
 * 5 digits (0..4) → 5³ possible triples = 125 states.
 */
#define STATE_SIZE   125
#define DIGIT_RANGE  5

/* ---------- state transition tables ----------
 * trans_valid[s][d] is true if appending digit d to the triple
 * represented by state s preserves the property.
 * trans_next[s][d] gives the state index of the new last three digits.
 */
static bool  trans_valid[STATE_SIZE][DIGIT_RANGE];
static int   trans_next [STATE_SIZE][DIGIT_RANGE];
static bool  trans_precomp_done = false;

/* Convert a triple (x1,x2,x3) to a unique state index 0..124. */
static inline int state_index(int x1, int x2, int x3)
{
    return (x1 * 25) + (x2 * 5) + x3;
}

/* Precompute the transition tables. */
static void precompute_transitions(void)
{
    if (trans_precomp_done) return;
    trans_precomp_done = true;

    for (int x1 = 0; x1 < DIGIT_RANGE; ++x1)
    for (int x2 = 0; x2 < DIGIT_RANGE; ++x2)
    for (int x3 = 0; x3 < DIGIT_RANGE; ++x3) {
        int s = state_index(x1, x2, x3);
        for (int d = 0; d < DIGIT_RANGE; ++d) {
            /* Check the three pair–sum inequalities in the window
             * (x1,x2,x3,d).  The pairs are (1,2) & (3,4), (1,3)&(2,4),
             * and (1,4)&(2,3). */
            int s12 = x1 + x2, s34 = x3 + d;
            int s13 = x1 + x3, s24 = x2 + d;
            int s14 = x1 + d,   s23 = x2 + x3;
            trans_valid[s][d] = (s12 != s34 && s13 != s24 && s14 != s23);
            trans_next [s][d] = state_index(x2, x3, d);
        }
    }
}

/* ---------- DP data ---------- */
static seq_t *state_cnt_cur   = NULL;   /* counts for current length */
static seq_t *state_cnt_next  = NULL;   /* temporary buffer for next length */
static size_t cur_len         = 0;      /* current length represented by cur state */
static size_t max_n_computed  = 0;      /* largest n for which a() is known */

/* ---------- sequence cache ---------- */
static seq_t *a_arr = NULL;   /* a_arr[0] unused, a_arr[n] holds a(n) for n≥1 */
static size_t a_cap = 0;       /* allocated size of a_arr */

/* Ensure the cache can hold values up to n. */
static void ensure_cache_capacity(size_t n)
{
    if (a_cap >= n + 1) return;
    size_t new_cap = n + 1;
    a_arr = (seq_t *)realloc(a_arr, sizeof(seq_t) * new_cap);
    /* If realloc failed, a_arr will be NULL – the caller will abort. */
    a_cap = new_cap;
}

/* Extend DP until we have computed a(n) for all 1 ≤ k ≤ n. */
static void compute_up_to(size_t n)
{
    if (n <= max_n_computed) return;

    precompute_transitions();
    ensure_cache_capacity(n);

    /* Initialise DP for length 3 (the first 3 digits – trivial). */
    if (cur_len == 0) {
        cur_len    = 3;
        state_cnt_cur  = (seq_t *)malloc(sizeof(seq_t) * STATE_SIZE);
        state_cnt_next = (seq_t *)malloc(sizeof(seq_t) * STATE_SIZE);
        for (int s = 0; s < STATE_SIZE; ++s) {
            state_cnt_cur[s]  = 1;   /* one way to have any particular triple */
            state_cnt_next[s] = 0;
        }
    }

    /* Extend until reaching length 3 + n. */
    while (cur_len < 3 + n) {
        /* Clear next buffer. */
        for (int s = 0; s < STATE_SIZE; ++s) state_cnt_next[s] = 0;

        /* For each current state, try all possible next digits. */
        for (int s = 0; s < STATE_SIZE; ++s) {
            seq_t cnt = state_cnt_cur[s];
            if (!cnt) continue;
            for (int d = 0; d < DIGIT_RANGE; ++d) {
                if (!trans_valid[s][d]) continue;
                int ns = trans_next[s][d];
                state_cnt_next[ns] += cnt;
            }
        }

        /* The new length is cur_len+1.  Its corresponding n value is k = (cur_len+1)-3. */
        size_t k = cur_len + 1 - 3;
        if (k >= 1 && k <= n) {
            seq_t total = 0;
            for (int s = 0; s < STATE_SIZE; ++s) total += state_cnt_next[s];
            a_arr[k] = total;
        }

        /* Swap buffers for next iteration. */
        seq_t *tmp = state_cnt_cur;
        state_cnt_cur = state_cnt_next;
        state_cnt_next = tmp;
        ++cur_len;
    }

    max_n_computed = n;
}

/* ---------- required public function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined outside offset – return 0 */
    compute_up_to((size_t)n);
    return a_arr[(size_t)n];
}
