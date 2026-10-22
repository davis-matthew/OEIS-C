/*
 * A323437  Number of semistandard Young tableaux whose entries are the prime
 *          indices of n.
 * https://oeis.org/A323437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to fill a Young diagram with the prime
 *            indices of n such that all rows are weakly increasing and all
 *            columns are strictly increasing
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/sieve_primes.h"

/* Forward declarations of helper functions */
static seq_t dfs_count(int pos, int m,
                       const int *shape, const int *row_start,
                       const int *row_of_idx, const int *col_of_idx,
                       int *board, const int *vals, int numVals, int *cnt);

static seq_t count_shape(const int *shape, int rows, int m,
                         const int *vals, int numVals, const int *cnt_orig);

static void enumerate_partitions(int rem, int max_part, int depth,
                                 int *shape, int m,
                                 const int *vals, int numVals,
                                 const int *cnt_orig, seq_t *total_out);

static seq_t compute_a_for_multiset(int m, const int *vals,
                                    int numVals, const int *cnt_orig);

/* Main sequence function */
seq_t a(idx_t n)
{
    /* One-time initialization of prime list and index lookup */
    static bool init = false;
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;
    static int *prime_index = NULL;

    if (!init) {
        sieve_primes(10000, &prime_list, &prime_cnt);
        size_t limit = 10000;
        prime_index = (int *) calloc(limit + 1, sizeof(int));
        for (size_t i = 0; i < prime_cnt; ++i) {
            if (prime_list[i] <= limit)
                prime_index[prime_list[i]] = (int)(i + 1); /* 1‑based index */
        }
        init = true;
    }

    /* Handle n = 0 or 1: empty multiset -> one tableau (empty) */
    if (n < 2)
        return (seq_t)1;

    /* Factor n (n <= 10000) by trial division using the prime list */
    uint64_t temp = n;
    int uniq_vals[20]  = {0};
    int counts[20]     = {0};
    int numVals = 0;
    int m = 0;          /* total number of prime indices (entries) */

    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            int idx = prime_index[p];
            int pos = -1;
            for (int j = 0; j < numVals; ++j)
                if (uniq_vals[j] == idx) { pos = j; break; }
            if (pos == -1) {
                uniq_vals[numVals] = idx;
                counts[numVals] = exp;
                ++numVals;
            } else
                counts[pos] += exp;
            m += exp;
        }
    }
    if (temp > 1) {           /* remaining prime factor */
        uint64_t p = temp;
        int idx = prime_index[p];
        int pos = -1;
        for (int j = 0; j < numVals; ++j)
            if (uniq_vals[j] == idx) { pos = j; break; }
        if (pos == -1) {
            uniq_vals[numVals] = idx;
            counts[numVals] = 1;
            ++numVals;
        } else
            counts[pos] += 1;
        ++m;
    }

    return compute_a_for_multiset(m, uniq_vals, numVals, counts);
}

/* ------------------------------------------------------------------ */
/* Recursive helper to count SSYT of a given shape and content */

static seq_t dfs_count(int pos, int m,
                       const int *shape, const int *row_start,
                       const int *row_of_idx, const int *col_of_idx,
                       int *board, const int *vals, int numVals, int *cnt)
{
    if (pos == m) return (seq_t)1;

    seq_t total = 0;
    int r = row_of_idx[pos];
    int c = col_of_idx[pos];

    int left_val = -1;
    if (c > 0)
        left_val = board[pos - 1];

    int above_val = -1;
    if (r > 0 && shape[r - 1] > c) {
        int above_idx = row_start[r - 1] + c;
        above_val = board[above_idx];
    }

    for (int k = 0; k < numVals; ++k) {
        if (cnt[k] == 0) continue;
        int val = vals[k];
        if (c > 0 && val < left_val) continue;
        if (r > 0 && shape[r - 1] > c && val <= above_val) continue;

        cnt[k]--;
        board[pos] = val;
        total += dfs_count(pos + 1, m, shape, row_start,
                           row_of_idx, col_of_idx,
                           board, vals, numVals, cnt);
        cnt[k]++;
    }
    return total;
}

static seq_t count_shape(const int *shape, int rows, int m,
                         const int *vals, int numVals, const int *cnt_orig)
{
    if (m == 0) return (seq_t)1;   /* empty diagram */

    int row_start[20];
    row_start[0] = 0;
    for (int i = 1; i < rows; ++i)
        row_start[i] = row_start[i - 1] + shape[i - 1];

    int row_of_idx[20];
    int col_of_idx[20];
    int idx = 0;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < shape[r]; ++c) {
            row_of_idx[idx] = r;
            col_of_idx[idx] = c;
            ++idx;
        }
    }

    int board[20];
    int cnt[20];
    memcpy(cnt, cnt_orig, numVals * sizeof(int));

    return dfs_count(0, m, shape, row_start,
                     row_of_idx, col_of_idx,
                     board, vals, numVals, cnt);
}

/* ------------------------------------------------------------------ */
/* Partition enumeration and accumulation of counts */

static void enumerate_partitions(int rem, int max_part, int depth,
                                 int *shape, int m,
                                 const int *vals, int numVals,
                                 const int *cnt_orig, seq_t *total_out)
{
    if (rem == 0) {
        seq_t ways = count_shape(shape, depth, m, vals, numVals, cnt_orig);
        *total_out += ways;
        return;
    }
    for (int part = (rem < max_part ? rem : max_part); part >= 1; --part) {
        shape[depth] = part;
        enumerate_partitions(rem - part, part, depth + 1,
                             shape, m, vals, numVals, cnt_orig, total_out);
    }
}

static seq_t compute_a_for_multiset(int m, const int *vals,
                                    int numVals, const int *cnt_orig)
{
    seq_t total = 0;
    int shape[20];
    enumerate_partitions(m, m, 0, shape, m, vals, numVals, cnt_orig, &total);
    return total;
}
