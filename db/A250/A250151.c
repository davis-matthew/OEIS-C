/*
 * A250151  Number of length n+6 0..5 arrays with every seven consecutive
 *          terms having the maximum of some two terms equal to the minimum of
 *          the remaining five terms.
 * https://oeis.org/A250151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 8, u64 n <= 24, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A250151
 *  Number of length n+6 0..5 arrays with every seven consecutive terms having the
 *  maximum of some two terms equal to the minimum of the remaining five terms.
 *  Computed via dynamic programming over the last six digits of the array.
 *  Each state is a 6‑tuple of digits in 0..5 (46656 states).
 *  For every state we precompute the list of digits that can be appended
 *  without violating the window condition.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* public API */
seq_t a(idx_t n)
{
    /* constants */
    enum { DIGITS = 6, STATE_SIZE = 46656, POW6_5 = 7776 };  /* 6⁵ = 7776 */

    /* transition tables (built once) */
    static unsigned char  trans_deg[STATE_SIZE];          /* allowed next digits per state   */
    static int            trans_next[STATE_SIZE][DIGITS];/* transition indices             */
    static bool           trans_ready = false;           /* flag: tables built?             */

    /* DP buffers */
    static seq_t *dp_cur = NULL;          /* counts for current length   */
    static seq_t *dp_nxt = NULL;          /* counts for next length     */

    /* memoised results for a(n)  (index 1..current_n) */
    static seq_t *results = NULL;         /* results[1..current_n]      */
    static size_t results_cap = 0;        /* allocated capacity         */
    static size_t current_n   = 0;        /* number of sequence terms   */

    /* ----------------------------------------------------
     * build transition tables once
     * ---------------------------------------------------- */
    if (!trans_ready) {
        /* for each state, determine which new digits keep the
         * window of seven valid.  state s is encoded as a base‑6
         * number representing digits a[0] … a[5] (most significant
         * first).  The next state after appending digit x is the
         * tuple a[1] … a[5] x.  */
        for (size_t s = 0; s < STATE_SIZE; ++s) {
            unsigned char a[6];
            size_t tmp = s;
            for (int i = 5; i >= 0; --i) {
                a[i] = (unsigned char)(tmp % DIGITS);
                tmp /= DIGITS;
            }

            unsigned char deg = 0;
            for (unsigned char x = 0; x < DIGITS; ++x) {
                /* construct the window of 7 digits */
                unsigned char w[7];
                for (int i = 0; i < 6; ++i) w[i] = a[i];
                w[6] = x;

                /* test whether this window satisfies the condition */
                bool ok = false;
                for (int i = 0; i < 7 && !ok; ++i) {
                    for (int j = i + 1; j < 7 && !ok; ++j) {
                        unsigned char max_ij = w[i] > w[j] ? w[i] : w[j];
                        unsigned char min_rest = 6;            /* greater than any digit */
                        for (int k = 0; k < 7; ++k) {
                            if (k == i || k == j) continue;
                            if (w[k] < min_rest) min_rest = w[k];
                        }
                        if (max_ij == min_rest) ok = true;
                    }
                }

                if (ok) {
                    /* compute next state index */
                    int nxt = (int)((s % POW6_5) * DIGITS + x);
                    trans_next[s][deg++] = nxt;
                }
            }
            trans_deg[s] = deg;
        }

        /* allocate DP buffers */
        dp_cur = (seq_t *)malloc(sizeof(seq_t) * STATE_SIZE);
        dp_nxt = (seq_t *)malloc(sizeof(seq_t) * STATE_SIZE);

        /* initial length = 6: every 6‑tuple is allowed, count = 1 */
        for (size_t s = 0; s < STATE_SIZE; ++s) dp_cur[s] = (seq_t)1;

        trans_ready = true;
    }

    /* ----------------------------------------------------
     * enlarge results array if needed
     * ---------------------------------------------------- */
    if (results_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;  /* indices 0..n   (index 0 unused) */
        seq_t *new_res = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (results) {
            memcpy(new_res, results, results_cap * sizeof(seq_t));
            free(results);
        }
        results = new_res;
        results_cap = new_cap;
    }

    /* ----------------------------------------------------
     * compute terms up to requested n
     * ---------------------------------------------------- */
    while (current_n < (size_t)n) {
        /* clear next DP buffer */
        for (size_t i = 0; i < STATE_SIZE; ++i) dp_nxt[i] = (seq_t)0;

        seq_t total_next = (seq_t)0;

        /* transition from each state */
        for (size_t s = 0; s < STATE_SIZE; ++s) {
            seq_t val = dp_cur[s];
            if (val == (seq_t)0) continue;
            unsigned char deg = trans_deg[s];
            for (unsigned char k = 0; k < deg; ++k) {
                int ns = trans_next[s][k];
                dp_nxt[ns] += val;
            }
            total_next += val * (seq_t)deg;
        }

        /* store result for length (current_n + 6 + 1) i.e. n = current_n + 1 */
        ++current_n;
        results[current_n] = total_next;

        /* prepare for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    return results[n];
}
