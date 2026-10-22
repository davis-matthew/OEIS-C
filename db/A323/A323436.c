/*
 * A323436  Number of plane partitions whose parts are the prime indices of n.
 * https://oeis.org/A323436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of plane partitions of the multiset of prime
 *            indices of n (indexing primes starting at 1). Implemented by
 *            enumerating all partition shapes and counting semi‑standard
 *            Young tableaux of that shape with the given content. For n ≤
 *            10000 the multiset size is ≤ 13, so brute force is fast
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/build_primes.h"
#include "../utils/factor_u64.h"

/* maximum number of cells (prime indices) for n ≤ 10000 */
#define MAX_M 20
/* primes up to 10000 are enough for all prime divisors of n ≤ 10000 */
#define PRIME_LIMIT 10000

/* cached prime array and index table (index 1‑based) */
static uint64_t *prime_list = NULL;      /* list of primes up to PRIME_LIMIT */
static size_t    prime_cnt  = 0;
static uint16_t  prime_index[PRIME_LIMIT + 1] = {0};

/* initialise prime tables once */
static void init_primes(void)
{
    if (prime_list != NULL) return;
    /* prime_list will be allocated by build_primes */
    build_primes((idx_t)PRIME_LIMIT, (idx_t **)&prime_list, &prime_cnt);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p <= PRIME_LIMIT) prime_index[(size_t)p] = (uint16_t)(i + 1); /* 1‑based index */
    }
}

/* data for a single shape λ (partition of m) */
static int    lambda_rows[MAX_M];          /* row lengths (non‑increasing) */
static int    lambda_len;                  /* number of rows */
static int    cell_r[MAX_M];               /* row index of cell k */
static int    cell_c[MAX_M];               /* column index of cell k */
static int    total_cells;                 /* total number of cells m */

/* grid holding current values during DFS (row major) */
static int grid[MAX_M][MAX_M];

/* data for multiset of prime indices of n */
static int uniq_vals[MAX_M];      /* distinct values */
static int uniq_counts[MAX_M];    /* remaining counts */
static int uniq_n;                /* number of distinct values */

/* DFS: count assignments for current shape */
static seq_t dfs_fill(int pos)
{
    if (pos == total_cells) return (seq_t)1; /* filled all cells */

    int r = cell_r[pos];
    int c = cell_c[pos];

    int left_val  = (c > 0) ? grid[r][c - 1] : INT_MAX;
    int top_val   = (r > 0 && c < lambda_rows[r - 1]) ? grid[r - 1][c] : INT_MAX;
    int max_allowed = (left_val < top_val) ? left_val : top_val;

    seq_t sum = 0;
    for (int i = 0; i < uniq_n; ++i) {
        int v = uniq_vals[i];
        if (uniq_counts[i] == 0 || v > max_allowed) continue;
        uniq_counts[i]--;
        grid[r][c] = v;
        sum += dfs_fill(pos + 1);
        grid[r][c] = 0; /* reset for backtracking */
        uniq_counts[i]++;
    }
    return sum;
}

/* count plane partitions for a shape λ */
static seq_t count_lambda(const int *parts, int len)
{
    lambda_len = len;
    for (int i = 0; i < len; ++i) lambda_rows[i] = parts[i];

    /* build cell positions (row major order) */
    total_cells = 0;
    for (int r = 0; r < len; ++r) {
        for (int c = 0; c < lambda_rows[r]; ++c) {
            cell_r[total_cells] = r;
            cell_c[total_cells] = c;
            ++total_cells;
        }
    }

    /* clear grid */
    for (int r = 0; r < len; ++r)
        for (int c = 0; c < lambda_rows[r]; ++c)
            grid[r][c] = 0;

    return dfs_fill(0);
}

/* generate all integer partitions of m and sum counts */
static seq_t partition_backtrack(int remaining, int max_part, int *parts, int parts_len)
{
    if (remaining == 0) return count_lambda(parts, parts_len);

    seq_t total = 0;
    if (max_part > remaining) max_part = remaining;
    for (int p = max_part; p >= 1; --p) {
        parts[parts_len] = p;
        total += partition_backtrack(remaining - p, p, parts, parts_len + 1);
    }
    return total;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n <= 1) return (seq_t)1;

    init_primes();

    /* factor n */
    uint64_t primes[MAX_M];
    uint64_t exps[MAX_M];
    size_t fac_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &fac_cnt);

    /* build multiset of prime indices (each exponent counts) */
    int multiset[MAX_M];
    int mult_len = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > PRIME_LIMIT) return (seq_t)0; /* should not occur for n ≤ 10000 */
        uint16_t idx = prime_index[(size_t)p]; /* 1‑based index of prime p */
        for (uint64_t e = 0; e < exps[i]; ++e) {
            multiset[mult_len++] = (int)idx;
            if (mult_len == MAX_M) break;
        }
    }

    if (mult_len == 0) return (seq_t)1;

    /* create distinct values and counts from multiset */
    uniq_n = 0;
    for (int i = 0; i < mult_len; ++i) {
        int v = multiset[i];
        bool seen = false;
        for (int j = 0; j < uniq_n; ++j) {
            if (uniq_vals[j] == v) {
                ++uniq_counts[j];
                seen = true;
                break;
            }
        }
        if (!seen) {
            uniq_vals[uniq_n] = v;
            uniq_counts[uniq_n] = 1;
            ++uniq_n;
        }
    }

    /* generate all partitions of m and sum */
    int parts[MAX_M];
    seq_t total = partition_backtrack(mult_len, mult_len, parts, 0);

    return total;
}
