/*
 * A323438  Number of ways to fill a Young diagram with the prime indices of n
 *          such that all rows and columns are weakly increasing.
 * https://oeis.org/A323438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for a given n, factor n to obtain the multiset of prime
   indices.  All Young diagram shapes (partitions) with that many boxes
   are enumerated.  For each shape the number of semistandard Young
   tableaux (weakly increasing rows and columns) with the given
   content is counted by a simple backtracking algorithm.  The sum
   over all shapes yields a(n). */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes.h"

#define MAX_CELLS 13               /* maximum total prime indices for n ≤ 10000 */

static idx_t *prime_list = NULL;
static size_t prime_cnt = 0;
static int   prime_index_of[10001] = {0};
static bool  prime_init = false;

/* DFS that counts SSYT for a fixed shape.
   grid   : 2‑D array with the current tableau (−1 for empty).
   counts : remaining multiplicities of each distinct prime index.
   pos    : next cell index in row‑major order.
   total  : total number of cells in the shape. */
static seq_t dfs_shape(int grid[MAX_CELLS][MAX_CELLS], int counts[MAX_CELLS],
                       size_t pos, size_t total,
                       const int *rows, size_t part_len,
                       const int *uniq_vals, size_t uniq_len) {
    if (pos == total) return 1;

    size_t r = 0, rem = pos;
    while (r < part_len && rem >= (size_t)rows[r]) {
        rem -= (size_t)rows[r];
        ++r;
    }
    size_t c = rem;

    int low = 0;
    if (c > 0) low = grid[r][c-1];
    if (r > 0 && c < (size_t)rows[r-1]) {
        int up = grid[r-1][c];
        if (up > low) low = up;
    }

    seq_t totalWays = 0;
    for (size_t i = 0; i < uniq_len; ++i) {
        if (counts[i] == 0) continue;
        int val = uniq_vals[i];
        if ((seq_t)val < (seq_t)low) continue;

        counts[i]--;
        grid[r][c] = val;
        totalWays += dfs_shape(grid, counts, pos + 1, total,
                               rows, part_len,
                               uniq_vals, uniq_len);
        grid[r][c] = -1;
        counts[i]++;        /* backtrack */
    }
    return totalWays;
}

/* Count the SSYT for a fixed shape described by rows[] (length part_len). */
static seq_t count_shape(const int *rows, size_t part_len,
                         const int *uniq_vals, const int *uniq_cnt,
                         size_t uniq_len) {
    int grid[MAX_CELLS][MAX_CELLS];
    for (size_t r = 0; r < part_len; ++r)
        for (size_t c = 0; c < (size_t)rows[r]; ++c)
            grid[r][c] = -1;

    int counts[MAX_CELLS];
    for (size_t i = 0; i < uniq_len; ++i) counts[i] = uniq_cnt[i];

    size_t total_cells = 0;
    for (size_t r = 0; r < part_len; ++r) total_cells += rows[r];

    return dfs_shape(grid, counts, 0, total_cells,
                     rows, part_len,
                     uniq_vals, uniq_len);
}

/* Generate all partitions of `remaining` into parts ≤ `max_part`
   and accumulate the SSYT counts into *accumulator. */
static void gen_partitions(int remaining, int max_part,
                           int *cur, size_t cur_len,
                           seq_t *accumulator,
                           const int *uniq_vals, const int *uniq_cnt,
                           size_t uniq_len) {
    if (remaining == 0) {
        *accumulator += count_shape(cur, cur_len,
                                    uniq_vals, uniq_cnt,
                                    uniq_len);
        return;
    }
    for (int part = (remaining < max_part ? remaining : max_part); part >= 1; --part) {
        cur[cur_len] = part;
        gen_partitions(remaining - part, part,
                       cur, cur_len + 1,
                       accumulator,
                       uniq_vals, uniq_cnt,
                       uniq_len);
    }
}

/* Public function implementing A323438. */
seq_t a(idx_t n) {
    if (!prime_init) {
        prime_init = true;
        build_primes((idx_t)10000, &prime_list, &prime_cnt);
        for (int i = 0; i <= 10000; ++i) prime_index_of[i] = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p <= 10000) prime_index_of[(int)p] = (int)(i + 1); /* 1‑based */
        }
    }

    if (n <= 1) return 1;     /* empty multiset → one (empty) tableau */

    uint64_t tmp = (uint64_t)n;
    int  uniq_vals[MAX_CELLS];
    int  uniq_cnt[MAX_CELLS];
    size_t uniq_len = 0;
    size_t total_cells = 0;

    for (size_t i = 0; i < prime_cnt && (prime_list[i] * prime_list[i]) <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) { tmp /= p; ++exp; }
            uniq_vals[uniq_len] = (int)(i + 1);  /* prime index */
            uniq_cnt[uniq_len] = exp;
            uniq_len++;
            total_cells += exp;
        }
    }
    if (tmp > 1) {  /* remaining prime factor */
        int idx = prime_index_of[(int)tmp];
        /* idx should never be zero for n ≤ 10000 */
        uniq_vals[uniq_len] = idx;
        uniq_cnt[uniq_len] = 1;
        uniq_len++;
        total_cells++;
    }

    int cur[MAX_CELLS];
    seq_t result = 0;
    gen_partitions((int)total_cells, (int)total_cells,
                   cur, 0, &result,
                   uniq_vals, uniq_cnt, uniq_len);
    return result;
}
