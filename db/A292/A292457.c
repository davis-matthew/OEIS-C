/*
 * A292457  Numbers where 7 outnumbers any other digit.
 * https://oeis.org/A292457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the n‑th smallest positive integer
   whose digit 7 occurs more times than any other digit. */

/* External utilities */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

static idx_t  n_global;
static idx_t  count_global;
static bool   found_global;
static seq_t  result_global;

/* Recursively generate all numbers of length L in increasing order
   and count those satisfying the digit 7 dominance property. */
static void dfs(idx_t pos, idx_t L, seq_t val, unsigned char cnt[10]) {
    if (found_global) return;
    if (pos == L) {
        /* Check the dominance condition */
        unsigned char max_other = 0;
        for (int d = 0; d < 10; ++d) {
            if (d != 7 && cnt[d] > max_other) max_other = cnt[d];
        }
        if (cnt[7] > max_other) {
            ++count_global;
            if (count_global == n_global) {
                result_global = val;
                found_global = true;
            }
        }
        return;
    }

    unsigned char start = (pos == 0) ? 1 : 0;  /* no leading zeros */
    for (unsigned char d = start; d <= 9; ++d) {
        ++cnt[d];
        seq_t new_val = val * 10 + d;
        idx_t remaining = L - pos - 1;

        /* Compute current maximum other digit count after this choice */
        unsigned char max_other = 0;
        for (int dd = 0; dd < 10; ++dd) {
            if (dd != 7 && cnt[dd] > max_other) max_other = cnt[dd];
        }

        /* Prune if even placing all remaining digits as 7 cannot beat
           the current maximum other count. */
        if (cnt[7] + remaining <= max_other) {
            --cnt[d];
            continue;
        }

        dfs(pos + 1, L, new_val, cnt);
        --cnt[d];
        if (found_global) return;
    }
}

seq_t a(idx_t n) {
    n_global     = n;
    count_global = 0;
    found_global = false;
    result_global = 0;

    unsigned char cnt[10] = {0};
    for (idx_t len = 1; !found_global; ++len) {
        dfs(0, len, 0, cnt);
    }
    return result_global;
}
