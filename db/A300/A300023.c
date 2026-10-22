/*
 * A300023  For any k, the cumulative sum a(1) + a(2) + a(3) + ... + a(k)
 *          shows at least a digit 9.
 * https://oeis.org/A300023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1118, u32 n <= 9132, u64 n <= 9132,
 *            u128 n <= 9132, big any n
 * Helpers:   has_nine
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300023 via greedy construction:
   Start with 9.  For each subsequent term pick the smallest unused
   positive integer such that the cumulative sum up to that term
   contains at least one digit 9.  The sequence is a permutation of
   the natural numbers. */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/has_nine.h"

/* static storage */
static seq_t *seq_buf = NULL;
static size_t seq_len = 0;
static seq_t cum_sum = 0;

static bool *used_buf = NULL;
static size_t used_cap = 0;

/* ensure used_buf can index up to need */
static void ensure_used_cap(size_t need) {
    if (need >= used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 16;
        while (new_cap <= need) new_cap <<= 1;
        bool *tmp = (bool *)realloc(used_buf, new_cap * sizeof(bool));
        if (!tmp) abort();
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
        used_buf = tmp;
        used_cap = new_cap;
    }
}

/* ensure seq_buf has space for at least need terms */
static void ensure_seq_cap(size_t need) {
    if (need > seq_len) {
        seq_t *tmp = (seq_t *)realloc(seq_buf, need * sizeof(seq_t));
        if (!tmp) abort();
        seq_buf = tmp;
        seq_len = need;
    }
}

/* public function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset 1 */
    size_t need = (size_t)n;
    if (need > seq_len) {
        size_t old_len = seq_len;
        ensure_seq_cap(need);
        for (size_t i = old_len; i < need; ++i) {
            idx_t cand = 1;
            for (;;) {
                ensure_used_cap((size_t)cand);
                if (!used_buf[cand]) {
                    seq_t cand_val = (seq_t)cand;
                    seq_t new_sum = cum_sum + cand_val;
                    if (has_nine(new_sum)) {
                        used_buf[cand] = true;
                        seq_buf[i] = cand_val;
                        cum_sum = new_sum;
                        break;
                    }
                }
                ++cand;
            }
        }
    }
    return seq_buf[n - 1];
}
