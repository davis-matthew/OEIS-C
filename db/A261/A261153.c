/*
 * A261153  a(n) is the maximum number of distinct primes whose sum is n.
 * https://oeis.org/A261153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximum number of distinct primes whose sum is n. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* DP table storing the best number of distinct primes for each n.
   best[0] = 0; for n > 0, best[n] = 0 if n cannot be represented
   by distinct primes, otherwise best[n] ≥ 1. */
static seq_t *best = NULL;
static idx_t best_max = 0;   /* highest n for which best[0..best_max] is valid */

/* Build an array of all primes ≤ max_val using a classic sieve.
   The caller must free the returned array. */
static uint64_t *build_primes_upto(idx_t max_val, size_t *cnt_out) {
    if (max_val < 2) {
        *cnt_out = 0;
        return NULL;
    }
    size_t size = (size_t)max_val + 1;
    bool *is_prime = (bool *)calloc(size, sizeof(bool));
    if (!is_prime) { *cnt_out = 0; return NULL; }

    for (idx_t i = 2; i < (idx_t)size; ++i) is_prime[i] = true;
    for (idx_t i = 2; i * i < (idx_t)size; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j < (idx_t)size; j += i) is_prime[j] = false;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i < (idx_t)size; ++i) if (is_prime[i]) ++cnt;

    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) { free(is_prime); *cnt_out = 0; return NULL; }

    size_t idx = 0;
    for (idx_t i = 2; i < (idx_t)size; ++i)
        if (is_prime[i]) primes[idx++] = (uint64_t)i;

    free(is_prime);
    *cnt_out = cnt;
    return primes;
}

/* Recompute the DP table up to new_max_n (inclusive). */
static void recompute_dp(idx_t new_max_n) {
    seq_t *new_best = (seq_t *)realloc(best, (size_t)(new_max_n + 1) * sizeof(seq_t));
    best = new_best;
    /* initialise all entries to 0 */
    for (idx_t i = 0; i <= new_max_n; ++i) best[i] = 0;
    best[0] = 0;

    /* Build primes up to new_max_n. */
    size_t prime_cnt;
    uint64_t *primes = build_primes_upto(new_max_n, &prime_cnt);

    /* 0‑1 knapsack DP: maximise the count of distinct primes. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = (idx_t)primes[pi];
        for (idx_t s = new_max_n; s >= p; --s) {
            idx_t prev = s - p;
            if (prev == 0 || best[prev]) {          /* reachable state */
                seq_t cand = best[prev] + (seq_t)1;
                if (cand > best[s]) best[s] = cand;
            }
        }
    }

    free(primes);
    best_max = new_max_n;
}

seq_t a(idx_t n) {
    if (best == NULL || n > best_max) {
        recompute_dp(n);
    }
    return best[n];   /* zero means n cannot be expressed by distinct primes */
}
