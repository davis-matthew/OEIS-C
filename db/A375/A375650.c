/*
 * A375650  a(n) is the cardinality of the sumset of the Collatz trajectory of
 *          n.
 * https://oeis.org/A375650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 6525, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) as the cardinality of the sumset of the Collatz trajectory of n */
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#include <algorithm>
#endif

/* Comparator for qsort (used only in C mode). */
static int cmp_seq_t_wrapper(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation (uses std::vector and std::sort) */
    std::vector<seq_t> col;
    seq_t cur = (seq_t)n;
    while (true) {
        col.push_back(cur);
        if (cur == (seq_t)1) break;
        if (cur & (seq_t)1) {
            cur = 3 * cur + 1;
        } else {
            cur = cur / 2;
        }
    }
    std::vector<seq_t> sums;
    const size_t len = col.size();
    sums.reserve(len * (len + 1) / 2);
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = i; j < len; ++j) {
            sums.push_back(col[i] + col[j]);
        }
    }
    std::sort(sums.begin(), sums.end());
    seq_t result = 0;
    if (!sums.empty()) {
        result = 1;
        seq_t prev = sums[0];
        for (size_t k = 1; k < sums.size(); ++k) {
            if (sums[k] != prev) {
                ++result;
                prev = sums[k];
            }
        }
    }
    return result;
#else
    /* C implementation (uses malloc/realloc and qsort) */
    size_t col_cap = 8, col_len = 0;
    seq_t *col = (seq_t *)malloc(col_cap * sizeof(seq_t));
    if (!col) return 0;
    seq_t cur = (seq_t)n;
    while (1) {
        if (col_len == col_cap) {
            col_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(col, col_cap * sizeof(seq_t));
            if (!tmp) { free(col); return 0; }
            col = tmp;
        }
        col[col_len++] = cur;
        if (cur == (seq_t)1) break;
        if (cur & (seq_t)1) {
            cur = 3 * cur + 1;
        } else {
            cur = cur / 2;
        }
    }
    size_t sum_cap = col_len * (col_len + 1) / 2;
    seq_t *sums = (seq_t *)malloc(sum_cap * sizeof(seq_t));
    if (!sums) { free(col); return 0; }
    size_t idx = 0;
    for (size_t i = 0; i < col_len; ++i) {
        for (size_t j = i; j < col_len; ++j) {
            sums[idx++] = col[i] + col[j];
        }
    }
    free(col);
    qsort(sums, sum_cap, sizeof(seq_t), cmp_seq_t_wrapper);
    seq_t result = 0;
    if (sum_cap > 0) {
        result = 1;
        seq_t prev = sums[0];
        for (size_t k = 1; k < sum_cap; ++k) {
            if (cmp_seq_t_wrapper(&sums[k], &prev) != 0) {
                ++result;
                prev = sums[k];
            }
        }
    }
    free(sums);
    return result;
#endif
}
