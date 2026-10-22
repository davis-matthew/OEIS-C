/*
 * A355745  Number of ways to choose a prime factor of each prime index of n
 *          (with multiplicity, in weakly increasing order) such that the
 *          result is also weakly increasing.
 * https://oeis.org/A355745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of weakly increasing choices of a prime factor of
 *            each prime index of n (with multiplicity) such that the chosen
 *            primes are also weakly increasing
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   cmp_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cmp_idx.h"

/* internal comparison for qsort */

/* ------------------------------------------------------------------ */
/* Prime table (dynamic, expanded on demand).  The array `primes` holds
   the list of primes, and `prime_index_map` maps a number to its
   1‑based index in that list (0 if not a prime). */
static uint32_t *primes = NULL;
static uint64_t *prime_index_map = NULL;
static size_t primes_cnt = 0;
static size_t primes_limit = 0;

/* Ensure that all primes up to N (inclusive) are available. */
static void ensure_primes_up_to(idx_t N)
{
    if (N <= (idx_t)primes_limit) return;

    /* Allocate sieve. */
    size_t sz = (size_t)N + 1;
    uint8_t *is_comp = (uint8_t*)calloc(sz, sizeof(uint8_t));
    if (!is_comp) exit(EXIT_FAILURE);

    for (idx_t i = 2; i * i <= N; ++i) {
        if (!is_comp[i]) {
            for (idx_t j = i * i; j <= N; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* Count primes and create mapping. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= N; ++i) if (!is_comp[i]) ++cnt;

    uint32_t *new_primes = (uint32_t*)malloc(cnt * sizeof(uint32_t));
    uint64_t *new_map = (uint64_t*)calloc(sz, sizeof(uint64_t));

    size_t pi = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            new_primes[pi] = (uint32_t)i;
            new_map[i] = pi + 1;      /* 1‑based */
            ++pi;
        }
    }

    free(is_comp);
    free(primes);
    free(prime_index_map);

    primes      = new_primes;
    prime_index_map = new_map;
    primes_cnt = cnt;
    primes_limit = (size_t)N;
}

/* Return the 1‑based index of a prime. The caller guarantees that p
   is prime; otherwise the result is 0. */
static idx_t get_prime_index(idx_t p)
{
    if (p > (idx_t)primes_limit) ensure_primes_up_to(p);
    return (idx_t)prime_index_map[p];
}

/* Factor a small integer into its distinct prime factors.
   The array `out` should be large enough; on return `len` contains
   the number of distinct factors (indices are 1‑based). */
static void factor_distinct(idx_t x, idx_t *out, size_t *len)
{
    *len = 0;
    if (x < 2) return;

    ensure_primes_up_to(x);
    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= x; ++i) {
        if (x % primes[i] == 0) {
            out[(*len)++] = (idx_t)primes[i];
            while (x % primes[i] == 0) x /= primes[i];
        }
    }
    if (x > 1) {
        out[(*len)++] = x;
    }
}

/* ------------------------------------------------------------------ */
/* Main function.  The implementation uses a dynamic programming
   approach over the sorted multiset of prime indices of n. */
seq_t a(idx_t n)
{
    /* Special case n = 1. */
    if (n == 1) return (seq_t)1;

    /* Ensure primes list is up to n for factorization of n. */
    ensure_primes_up_to(n);

    /* Factor n, building the multiset of prime indices. */
    uint32_t local_primes[32];
    int     local_exps[32];
    size_t  local_cnt = 0;
    idx_t   tmp = n;

    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i) {
        if (tmp % primes[i] == 0) {
            int e = 0;
            while (tmp % primes[i] == 0) {
                tmp /= primes[i];
                ++e;
            }
            local_primes[local_cnt] = (uint32_t)primes[i];
            local_exps[local_cnt]   = e;
            ++local_cnt;
        }
    }
    if (tmp > 1) {
        local_primes[local_cnt] = (uint32_t)tmp;
        local_exps[local_cnt]   = 1;
        ++local_cnt;
    }

    /* Build the multiset of indices (with multiplicity). */
    idx_t indices[64];
    size_t idx_len = 0;
    for (size_t i = 0; i < local_cnt; ++i) {
        idx_t prime = (idx_t)local_primes[i];
        idx_t idx = get_prime_index(prime);
        for (int e = 0; e < local_exps[i]; ++e) {
            indices[idx_len++] = idx;
        }
    }

    if (idx_len == 0)          /* n == 1 handled above, but guard. */
        return (seq_t)1;

    /* Sort indices. */
    qsort(indices, idx_len, sizeof(idx_t), cmp_idx);

    /* DP arrays.  dp_primes[j] holds a prime factor from the
       previous index with dp_counts[j] ways to reach it. */
    idx_t dp_primes[32];
    seq_t dp_counts[32];
    size_t dp_len = 0;

    /* Process the first index. */
    idx_t first_m = indices[0];
    if (first_m == 1) return (seq_t)0;   /* index 1 has no prime factors */

    idx_t factors[32];
    size_t f_len = 0;
    factor_distinct(first_m, factors, &f_len);

    if (f_len == 0) return (seq_t)0;   /* should not happen except m==1 */

    for (size_t k = 0; k < f_len; ++k) {
        dp_primes[k] = factors[k];
        dp_counts[k] = (seq_t)1;
    }
    dp_len = f_len;

    /* Process remaining indices. */
    for (size_t pos = 1; pos < idx_len; ++pos) {
        idx_t cur_m = indices[pos];

        if (cur_m == 1) return (seq_t)0;   /* index 1 kills result */

        idx_t cur_factors[32];
        size_t cf_len = 0;
        factor_distinct(cur_m, cur_factors, &cf_len);

        if (cf_len == 0) return (seq_t)0;   /* should not happen */

        seq_t new_counts[32];
        size_t new_len = 0;

        /* For each factor of the current index, sum previous counts
           with primes <= this factor. */
        for (size_t i = 0; i < cf_len; ++i) {
            idx_t prime_val = cur_factors[i];
            seq_t sum = (seq_t)0;
            for (size_t j = 0; j < dp_len; ++j) {
                if (dp_primes[j] <= prime_val)
                    sum += dp_counts[j];
                else
                    break;           /* dp_primes sorted ascending */
            }
            new_counts[new_len++] = sum;
        }

        /* Update DP arrays for next iteration. */
        for (size_t i = 0; i < new_len; ++i) {
            dp_primes[i] = cur_factors[i];
            dp_counts[i] = new_counts[i];
        }
        dp_len = new_len;
        if (dp_len == 0) return (seq_t)0;   /* no ways left */
    }

    /* Sum final counts. */
    seq_t total = (seq_t)0;
    for (size_t i = 0; i < dp_len; ++i)
        total += dp_counts[i];

    return total;
}
