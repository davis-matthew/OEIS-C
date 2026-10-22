/*
 * A307223  Irregular table T(n, k) read by rows: n-th row gives number of
 *          subsets of the divisors of n which sum to k for 1 <= k <=
 *          sigma(n).
 * https://oeis.org/A307223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r, k) where r and k are obtained by scanning the
 *            irregular table of divisor–sum subsets row‑wise. r = smallest
 *            integer with Σ_{i=1}^{r} σ(i) ≥ n, k = n – Σ_{i=1}^{r-1} σ(i) (1
 *            ≤ k ≤ σ(r))
 * Range:     u8 n <= 8299, u16 n <= 8299, u32 n <= 8299, u64 n <= 8299,
 *            u128 n <= 8299, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* global state for memoisation */
static idx_t *cum_sigma = NULL;   /* prefix sums of σ(i) */
static size_t cum_size = 0;       /* current number of elements in cum_sigma */
static seq_t **dp_cache = NULL;   /* dp_cache[r] = table for row r, 1‑based */
static size_t *dp_len   = NULL;   /* number of entries in each dp array */
static size_t max_rows  = 0;      /* allocated size of the above arrays */

/* ensure that rows are computed up to reach cumulative σ >= target */
static void ensure_rows_up_to_target(idx_t target)
{
    /* initialise structure on first use */
    if (cum_sigma == NULL) {
        cum_size = 1;
        max_rows = 1;
        cum_sigma = (idx_t *)malloc(sizeof(idx_t) * max_rows);
        cum_sigma[0] = 0;
        dp_cache = (seq_t **)malloc(sizeof(seq_t *) * max_rows);
        dp_len   = (size_t   *)malloc(sizeof(size_t   ) * max_rows);
        dp_cache[0] = NULL;
    }

    /* compute rows until we cover the target index */
    while (cum_sigma[cum_size - 1] < target) {
        idx_t row = cum_size;   /* next row number to generate */

        /* factorise 'row' by trial division */
        uint64_t n_tmp = (uint64_t)row;
        uint64_t primes_buf[64];
        int     exp_buf   [64];
        size_t  pcnt      = 0;
        for (uint64_t p = 2; p <= n_tmp / p; ++p) {
            if (n_tmp % p == 0) {
                int e = 0;
                while (n_tmp % p == 0) { n_tmp /= p; ++e; }
                primes_buf[pcnt] = p;
                exp_buf   [pcnt] = e;
                ++pcnt;
            }
        }
        if (n_tmp > 1) {
            primes_buf[pcnt] = n_tmp;
            exp_buf   [pcnt] = 1;
            ++pcnt;
        }

        /* generate all divisors of 'row' */
        uint64_t *divs = NULL;
        size_t   div_cap = 1, div_cnt = 1;
        divs = (uint64_t *)malloc(sizeof(uint64_t) * div_cap);
        divs[0] = 1;
        for (size_t fi = 0; fi < pcnt; ++fi) {
            uint64_t prime = primes_buf[fi];
            int      exp   = exp_buf   [fi];
            size_t   cur_cnt = div_cnt;
            for (size_t j = 0; j < cur_cnt; ++j) {
                uint64_t base = divs[j];
                uint64_t val  = base;
                for (int e = 1; e <= exp; ++e) {
                    val *= prime;
                    if (div_cnt == div_cap) {
                        div_cap *= 2;
                        divs = (uint64_t *)realloc(divs, sizeof(uint64_t) * div_cap);
                    }
                    divs[div_cnt++] = val;
                }
            }
        }

        /* compute σ(row) from all divisors */
        seq_t sigma_seq = 0;
        for (size_t i = 0; i < div_cnt; ++i) sigma_seq += (seq_t)divs[i];
        idx_t sigma = (idx_t)sigma_seq;  /* assume it fits in idx_t */

        /* allocate DP table for this row */
        size_t dp_capacity = sigma + 1;
        seq_t *dp = (seq_t *)calloc(dp_capacity, sizeof(seq_t));
        dp[0] = 1;
        for (size_t di = 0; di < div_cnt; ++di) {
            uint64_t d = divs[di];
            for (size_t s = dp_capacity - 1; s >= (size_t)d; --s) {
                dp[s] += dp[s - (size_t)d];
            }
        }
        free(divs);

        /* grow global arrays if necessary */
        if (cum_size >= max_rows) {
            max_rows = max_rows ? max_rows << 1 : 4;
            cum_sigma = (idx_t *)realloc(cum_sigma, sizeof(idx_t) * max_rows);
            dp_cache = (seq_t **)realloc(dp_cache, sizeof(seq_t *) * max_rows);
            dp_len   = (size_t   *)realloc(dp_len   , sizeof(size_t   ) * max_rows);
        }

        cum_sigma[cum_size] = cum_sigma[cum_size - 1] + sigma;
        dp_cache[cum_size]  = dp;
        dp_len   [cum_size] = dp_capacity;
        ++cum_size;
        /* loops until cum_sigma[cum_size-1] >= target */
    }
}

/* required function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based; return 0 for safety */

    /* make sure we have rows up to this flat index */
    ensure_rows_up_to_target(n);

    /* binary search: find row r with cum_sigma[r-1] < n <= cum_sigma[r] */
    idx_t low = 1, high = (idx_t)(cum_size - 1);
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (cum_sigma[mid] >= n) high = mid;
        else                     low  = mid + 1;
    }
    idx_t r = low;

    /* column within that row */
    idx_t offset = n - cum_sigma[r - 1];  /* 1 ≤ offset ≤ σ(row) */

    /* return the DP value */
    return dp_cache[r][(size_t)offset];
}
