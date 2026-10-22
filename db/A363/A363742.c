/*
 * A363742  Number of integer factorizations of n with different mean, median,
 *          and mode.
 * https://oeis.org/A363742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer factorizations of n into integers >1
 *            for which the mean, the median, and the (unique) mode are all
 *            pairwise distinct. If a factorization has multiple modes, it is
 *            considered automatically different from the mean and the median,
 *            so only the mean ≠ median condition applies
 * Range:     u8 n <= 4319, u16 n <= 9983, u32 n <= 9983, u64 n <= 9983,
 *            u128 n <= 9983, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Recursive enumeration of factors in non‑decreasing order. */
static void dfs(uint64_t rem, uint64_t start, uint64_t sum, size_t len,
                seq_t *cnt, uint64_t *fac)
{
    if (rem == 1) {
        if (len == 0) return;               /* product 1 has no factors */
        /* Mean */
        seq_t mean_num = sum;
        seq_t mean_den = len;
        /* Median */
        seq_t median_num, median_den;
        if (len & 1U) {
            median_num = fac[len >> 1];
            median_den = 1;
        } else {
            median_num = fac[(len >> 1) - 1] + fac[len >> 1];
            median_den = 2;
        }
        /* Mean equals Median? */
        bool mean_eq_median =
            (mean_num * median_den == median_num * mean_den);
        /* Mode detection */
        uint64_t mode_val = fac[0];
        int mode_count  = 1;   /* number of values achieving the max freq */
        int max_freq    = 1;
        int curr_freq   = 1;
        for (size_t i = 1; i < len; ++i) {
            if (fac[i] == fac[i - 1]) {
                curr_freq++;
            } else {
                if (curr_freq > max_freq) {
                    max_freq   = curr_freq;
                    mode_count = 1;
                    mode_val   = fac[i - 1];
                } else if (curr_freq == max_freq) {
                    mode_count++;
                }
                curr_freq = 1;
            }
        }
        if (curr_freq > max_freq) {
            max_freq   = curr_freq;
            mode_count = 1;
            mode_val   = fac[len - 1];
        } else if (curr_freq == max_freq) {
            mode_count++;
        }

        bool unique_mode = (mode_count == 1);
        bool mean_eq_mode  = unique_mode &&
                             (mean_num == (seq_t)mode_val * mean_den);
        bool median_eq_mode = unique_mode &&
                               (median_num == (seq_t)mode_val * median_den);

        /* All three must be distinct */
        bool ok = !mean_eq_median;
        if (unique_mode) {
            ok &= !mean_eq_mode;
            ok &= !median_eq_mode;
        }
        if (ok) (*cnt)++;
        return;
    }

    for (uint64_t f = start; f <= rem; ++f) {
        if (rem % f == 0) {
            fac[len] = f;
            dfs(rem / f, f, sum + f, len + 1, cnt, fac);
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    uint64_t N = (uint64_t)n;
    uint64_t fac[64];
    seq_t cnt = 0;
    dfs(N, 2, 0, 0, &cnt, fac);
    return cnt;
}
