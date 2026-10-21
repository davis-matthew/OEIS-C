/*
 * A282288  Expansion of (Sum_{k>=0} x^(k^4))^19.
 * https://oeis.org/A282288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 471, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A282288: Number of ordered 19‑tuples of fourth powers summing to n */
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Computes a(n) for the sequence A282288. */
seq_t a(idx_t n)
{
    /* Cache of previously computed results */
    static seq_t *dp = NULL;
    static idx_t max_n = 0;

    /* Recompute when requested n exceeds cached value or cache is empty. */
    if (dp == NULL || n > max_n) {
        idx_t new_max = n;

        /* Pre‑compute all k^4 ≤ new_max.  The count is tiny for reasonable n. */
        idx_t powers[1024];
        size_t pcnt = 0;
        for (idx_t k = 0; ; ++k) {
            idx_t p = k * k * k * k;
            if (p > new_max) break;
            powers[pcnt++] = p;
        }

        /* Dynamic programming with 19 convolutions. */
        seq_t *cur = (seq_t*)calloc(new_max + 1, sizeof(seq_t));
        if (!cur) return 0;          /* allocation failed – unlikely in tests */
        cur[0] = 1;                  /* base: zero‑sum with zero terms */

        for (int step = 1; step <= 19; ++step) {
            seq_t *next = (seq_t*)calloc(new_max + 1, sizeof(seq_t));
            if (!next) { free(cur); return 0; }

            for (size_t pi = 0; pi < pcnt; ++pi) {
                idx_t val = powers[pi];
                for (idx_t s = val; s <= new_max; ++s) {
                    next[s] += cur[s - val];
                }
            }

            free(cur);
            cur = next;
        }

        /* Store the new table in the cache, discarding the old one. */
        if (dp) free(dp);
        dp = cur;
        max_n = new_max;
    }

    return dp[n];
}
