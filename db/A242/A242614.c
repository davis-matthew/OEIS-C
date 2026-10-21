/*
 * A242614  Triangle read by rows: row n contains numbers with sum of digits =
 *          n, and not greater than the n-th repunit (cf. A007953 and
 *          A002275).
 * https://oeis.org/A242614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 317, u32 n <= 5555, u64 n <= 5555,
 *            u128 n <= 5555, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A242614 – Triangle read by rows
   Numbers with digit sum = n, not greater than the nth repunit.
   The function a(idx_t n) returns a(n) using combinatorial
   generation and dynamic programming for digit sums. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"

static seq_t* dp0_data = NULL;       /* dp0[sum][len]  : 0‑9 digits, leading zeros allowed */
static size_t  dp_dim   = 0;          /* current size of dp array (dp_dim = max_r+1) */

static void ensure_dp(size_t maxR) {
    if (dp_dim > maxR) return;

    size_t new_dim = maxR + 1;
    seq_t* new_data = (seq_t*)malloc(new_dim * new_dim * sizeof(seq_t));
    if (!new_data) return;  /* failure – caller will handle */

    for (size_t s = 0; s < new_dim; ++s)
        for (size_t l = 0; l < new_dim; ++l)
            new_data[s * new_dim + l] = 0;

    /* DP base: sum 0 with 0 digits */
    new_data[0 * new_dim + 0] = 1;

    for (size_t l = 1; l < new_dim; ++l) {
        for (size_t s = 0; s < new_dim; ++s) {
            seq_t total = 0;
            for (int d = 0; d <= 9; ++d) {
                if (s >= (size_t)d)
                    total += new_data[(s - d) * new_dim + (l - 1)];
            }
            new_data[s * new_dim + l] = total;
        }
    }

    if (dp0_data) free(dp0_data);
    dp0_data = new_data;
    dp_dim   = new_dim;
}

static inline seq_t dp0_at(size_t sum, size_t len) {
    return dp0_data[sum * dp_dim + len];
}

/* Row count cache: A242622(n) – number of terms in row n */
static seq_t* rowCount_cache = NULL;
static size_t rowCache_size  = 0;

static seq_t count_len_nonzero_seq(size_t sum, int len) {
    if (len == 1) return (sum <= 9) ? ((seq_t)1) : ((seq_t)0);
    int max_first = (sum > 9) ? 9 : (int)sum;
    seq_t tot = 0;
    for (int d = 1; d <= max_first; ++d)
        tot += dp0_at(sum - d, len - 1);
    return tot;
}

static void ensure_row_cache(size_t maxR) {
    if (rowCache_size > maxR) return;

    ensure_dp(maxR);

    size_t newSize = maxR + 1;
    seq_t* newArr = (seq_t*)malloc(newSize * sizeof(seq_t));
    if (!newArr) return;  /* failure */

    for (size_t i = 0; i < newSize; ++i) newArr[i] = 0;

    newArr[0] = 1;  /* A242622(0) = 1 */

    for (size_t r = 1; r <= maxR; ++r) {
        seq_t sum_counts = 0;
        for (int L = 1; L < (int)r; ++L)
            sum_counts += count_len_nonzero_seq(r, L);
        newArr[r] = newArr[r - 1] + sum_counts;
    }

    if (rowCount_cache) free(rowCount_cache);
    rowCount_cache = newArr;
    rowCache_size = newSize;
}

static seq_t a_row(idx_t r, idx_t pos) {
    if (r == 0) return 0;  /* only term 0 in row 0 */
    ensure_row_cache(r);

    size_t pos_rem = (size_t)pos;

    /* handle lengths < r first */
    for (int L = 1; L < (int)r; ++L) {
        seq_t count_L = count_len_nonzero_seq(r, L);
        size_t count_U = (size_t)count_L;
        if (pos_rem < count_U) {
            /* build number of length L (< r) */
            size_t rem_sum = r;
            size_t positions = L;
            seq_t value = 0;
            for (size_t pos_i = 1; pos_i <= L; ++pos_i) {
                int d_start = (pos_i == 1) ? 1 : 0;
                int d_end   = (rem_sum > 9) ? 9 : (int)rem_sum;
                for (int d = d_start; d <= d_end; ++d) {
                    size_t avail = (size_t)dp0_at(rem_sum - d,
                                                 positions - 1);
                    if (pos_rem < avail) {
                        value = value * ((seq_t)10) + (seq_t)d;
                        rem_sum -= d;
                        --positions;
                        break;
                    } else {
                        pos_rem -= avail;
                    }
                }
            }
            return value;
        }
        pos_rem -= (size_t)count_L;
    }

    /* length == r */
    seq_t count_L = rowCount_cache[r - 1];
    size_t count_U = (size_t)count_L;
    if (pos_rem < count_U) {
        seq_t suffix = a_row(r - 1, pos_rem);
        seq_t powval = pow10(r - 1);
        return powval * ((seq_t)1) + suffix;
    }
    return 0;  /* should not happen */
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    idx_t cumulative = 0;
    idx_t r = 0;
    while (1) {
        ensure_row_cache(r);
        seq_t rowCnt = rowCount_cache[r];
        idx_t cntU = (idx_t)rowCnt;
        if (cumulative + cntU > n) break;
        cumulative += cntU;
        ++r;
    }

    idx_t idx_in_row = n - cumulative;  /* 0‑based index within row r */
    return a_row(r, idx_in_row);
}
