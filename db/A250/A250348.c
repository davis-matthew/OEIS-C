/*
 * A250348  Number of length n arrays x(i), i=1..n with x(i) in i..i+5 and no
 *          value appearing more than 2 times.
 * https://oeis.org/A250348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of length n arrays x(i) with i ≤ x(i) ≤ i+5 and each
 *            value occurring at most twice. Computation by a breadth‑first DP
 *            with 3^6 states (each representing 0,1,2 occurrences of the
 *            currently active 6 values). The state is represented in base‑3.
 *            The recurrence is not directly used; instead the...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

#define NODES 729            /* 3^6 */

static int trans[NODES][6];

/* Pre‑compute state transitions once. */
static void init_trans(void) {
    for (int s = 0; s < NODES; ++s) {
        unsigned char c[6];
        int tmp = s;
        for (int k = 0; k < 6; ++k) {
            c[k] = tmp % 3;
            tmp /= 3;
        }
        for (int j = 0; j < 6; ++j) {
            if (c[j] >= 2) {
                trans[s][j] = -1;
                continue;
            }
            /* Increment count at position j. */
            unsigned char d[6];
            for (int k = 0; k < 6; ++k) d[k] = c[k];
            d[j]++;

            /* Shift window: drop leftmost, add zero at right. */
            unsigned char e[6];
            e[0] = d[1];
            e[1] = d[2];
            e[2] = d[3];
            e[3] = d[4];
            e[4] = d[5];
            e[5] = 0;

            int idx = e[0];
            idx += 3 * (e[1] + 3 * (e[2] + 3 * (e[3] + 3 * (e[4] + 3 * e[5]))));
            trans[s][j] = idx;
        }
    }
}

/* Global static buffers used by the DP. */
static seq_t *dp_arr = NULL;        /* current state counts */
static seq_t *next_arr = NULL;      /* next state counts */
static idx_t cur_n = 0;             /* number of steps already processed */
static seq_t *cache = NULL;         /* cache of a(k) values for k ≥ 1 */
static idx_t cache_cap = 0;         /* capacity of cache (max n cached) */
static int trans_initialized = 0;

/* Ensure that cache contains a(k) for all k ≤ target. */
static void ensure_cache(idx_t target) {
    if (!trans_initialized) {
        init_trans();
        trans_initialized = 1;
    }

    if (dp_arr == NULL) {
        dp_arr = alloc_seq_array(NODES);
        next_arr = alloc_seq_array(NODES);
        /* Initially all entries are zero; set state 0 to 1. */
        dp_arr[0] = (seq_t)1;
    }

    /* Grow cache if necessary. */
    if (target > cache_cap) {
        seq_t *new_cache = alloc_seq_array(target + 1);
        /* Copy existing values. */
        if (cache != NULL) {
            for (idx_t i = 1; i <= cache_cap; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        cache = new_cache;
        cache_cap = target;
    }

    /* Process DP steps until reaching target. */
    while (cur_n < target) {
        /* Prepare next_arr for the new step. */
        for (int s = 0; s < NODES; ++s) next_arr[s] = (seq_t)0;

        for (int s = 0; s < NODES; ++s) {
            seq_t val = dp_arr[s];
            for (int j = 0; j < 6; ++j) {
                int t = trans[s][j];
                if (t != -1) {
                    next_arr[t] += val;
                }
            }
        }

        /* Sum all counts: this is a(cur_n+1). */
        seq_t sum = (seq_t)0;
        for (int s = 0; s < NODES; ++s) sum += next_arr[s];
        cache[cur_n + 1] = sum;

        /* Prepare for next iteration. */
        seq_t *tmp = dp_arr;
        dp_arr = next_arr;
        next_arr = tmp;
        cur_n++;
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
