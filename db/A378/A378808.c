/*
 * A378808  Numbers with monotonically decreasing digits, decreasing by only 0
 *          or 1.
 * https://oeis.org/A378808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code computes the nth number in OEIS A378808:
   numbers with monotonically decreasing digits, each step decreasing by at most 1.
   It uses combinatorial counting to locate the desired number without enumerating all
   predecessors. */

#include <stdint.h>
#include <stdbool.h>

static void init_binom(void);

static seq_t binom[21][21];
static bool binom_initialized = false;

static void init_binom(void)
{
    if (!binom_initialized) {
        for (idx_t n = 0; n < 21; ++n) {
            binom[n][0] = binom[n][n] = 1;
            for (idx_t k = 1; k < n; ++k) {
                binom[n][k] = binom[n - 1][k - 1] + binom[n - 1][k];
            }
        }
        binom_initialized = true;
    }
}

static seq_t count_remaining(idx_t len, idx_t cur)
{
    if (len == 0) return 1;
    seq_t sum = 0;
    idx_t max_t = (cur < len) ? cur : len;
    for (idx_t t = 0; t <= max_t; ++t) {
        sum += binom[len][t];
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Handle the sequence in O(L) time, where L is the number of digits
       (L <= 11 for n up to 10000). */
    init_binom();

    if (n == 0) return 0;  /* undefined, but return 0 for safety */

    seq_t cumulative = 0;
    idx_t L = 0;

    /* Find the length of the nth number. */
    while (true) {
        ++L;
        seq_t count_len = 0;
        for (idx_t d0 = 1; d0 <= 9; ++d0) {
            count_len += count_remaining(L - 1, d0);
        }
        if (cumulative + count_len >= n) break;
        cumulative += count_len;
    }

    seq_t rank = n - cumulative - 1;   /* zero‑based rank within numbers of length L */

    /* Determine the first digit. */
    idx_t cur_digit = 0;
    seq_t result = 0;
    idx_t remaining_len = L - 1;

    for (idx_t d0 = 1; d0 <= 9; ++d0) {
        seq_t cnt = count_remaining(L - 1, d0);
        if (rank < cnt) {
            cur_digit = d0;
            result = d0;
            break;
        } else {
            rank -= cnt;
        }
    }

    /* Build the remaining digits. */
    while (remaining_len > 0) {
        idx_t options[2];
        idx_t opt_cnt = 0;
        if (cur_digit > 0) {
            options[opt_cnt++] = cur_digit - 1;   /* smaller digit first */
        }
        options[opt_cnt++] = cur_digit;          /* then the same digit */

        for (idx_t o = 0; o < opt_cnt; ++o) {
            idx_t nd = options[o];
            seq_t cnt_sub = count_remaining(remaining_len - 1, nd);
            if (rank < cnt_sub) {
                result = result * 10 + (seq_t)nd;
                cur_digit = nd;
                remaining_len -= 1;
                break;
            } else {
                rank -= cnt_sub;
            }
        }
    }

    return result;
}
