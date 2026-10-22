/*
 * A379930  Numbers that have the same number of divisors as there are runs in
 *          their base-10 representation.
 * https://oeis.org/A379930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 5500, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_runs_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th integer k such that the number of divisors of k
 * equals the number of runs of consecutive identical decimal digits of k.
 * The function evaluates successive integers, tests the equality, and
 * stores the results in a static cache. */

/* compiler‐agnostic includes */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_runs_uint64.h"

/* local helper: compute divisor count of a uint64_t integer by trial div */
static inline int divisor_count_uint64(uint64_t n)
{
    int cnt = 1;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int exp = 1;
            n /= p;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            cnt *= exp + 1;
        }
    }
    if (n > 1) cnt *= 2;        /* remaining prime factor */
    return cnt;
}

/* static cache for found terms */
static seq_t *seq_cache = NULL;
static idx_t  seq_len   = 0;
static size_t seq_cap   = 0;

/* next integer candidate to test */
static uint64_t next_candidate = 1;

/* function required by the problem statement */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; behaviour below is arbitrary */

    /* grow cache until we have at least n terms */
    while (seq_len < n) {
        /* ensure capacity of array */
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            seq_cache = tmp;
            seq_cap   = new_cap;
        }

        uint64_t cand = next_candidate++;
        int div_cnt   = divisor_count_uint64(cand);
        int run_cnt   = count_runs_uint64(cand);
        if (div_cnt == run_cnt) {
            seq_cache[seq_len++] = (seq_t)cand;
        }
    }
    return seq_cache[n - 1];
}
