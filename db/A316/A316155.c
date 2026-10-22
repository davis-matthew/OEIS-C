/*
 * A316155  Numbers with additive persistence = 4 and digits in nondecreasing
 *          order.
 * https://oeis.org/A316155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose decimal digits are in *
 *            nondecreasing order and whose digit sum equals 199 * (hence
 *            additive persistence = 4). The terms are listed in * increasing
 *            numeric order, starting at a(1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 10000,
 *            big any n
 * Helpers:   add_sat
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/add_sat.h"

/* Maximum possible digit sum in this sequence */
#define SUM 199

/* DP[d][r][l] = number of weakly increasing sequences of length l,
 * consisting of digits >= d, whose sum is r.
 * d ranges 1..10 (10 is a sentinel), r 0..SUM, l 0..SUM.           */
static uint64_t DP[11][200][200];
static bool dp_inited = false;

/* Pre‑compute the DP table once. */
static void init_dp(void)
{
    if (dp_inited) return;
    dp_inited = true;

    for (int r = 0; r <= SUM; ++r) {
        for (int l = 0; l <= SUM; ++l) {
            DP[10][r][l] = (r == 0 && l == 0) ? 1ULL : 0ULL;
        }
    }

    for (int d = 9; d >= 1; --d) {
        for (int r = 0; r <= SUM; ++r) {
            for (int l = 0; l <= SUM; ++l) {
                uint64_t ways = 0ULL;
                int maxk = r / d;
                if (maxk > l) maxk = l;
                for (int k = 0; k <= maxk; ++k) {
                    ways = add_sat(ways, DP[d + 1][r - k * d][l - k]);
                }
                DP[d][r][l] = ways;
            }
        }
    }
}

/* Convert a decimal string to seq_t */
static seq_t str_to_seq(const char *s)
{
    seq_t value = 0;
    while (*s) {
        value = value * 10 + (seq_t)(*s - '0');
        ++s;
    }
    return value;
}

/* Return the nth term of the sequence, for n >= 1. */
seq_t a(idx_t n)
{
    init_dp();

    /* Determine the length L of the desired number. */
    idx_t remaining = n;          /* 1‑based index within current length */
    int L = -1;
    int min_len = (SUM + 8) / 9;   /* ceil(SUM / 9) */
    int max_len = SUM;             /* ones only */

    for (int len = min_len; len <= max_len; ++len) {
        uint64_t count = DP[1][SUM][len];
        if (remaining > count) {
            remaining -= count;
        } else {
            L = len;
            break;
        }
    }

    if (L == -1) return (seq_t)0; /* should not occur for valid inputs */

    /* Build the digits of the nth number of length L. */
    int rem_sum = SUM;
    int rem_len = L;

    /* Temporary buffer for the decimal string (max 199 digits). */
    char buf[200];
    size_t pos = 0;

    for (int d = 1; d <= 9; ++d) {
        int maxk = rem_sum / d;
        if (maxk > rem_len) maxk = rem_len;

        for (int k = maxk; k >= 0; --k) {
            uint64_t count = DP[d + 1][rem_sum - k * d][rem_len - k];
            if (remaining > count) {
                remaining -= count;
            } else {
                /* Append k copies of digit d. */
                for (int t = 0; t < k; ++t) {
                    buf[pos++] = '0' + d;
                }
                rem_sum -= k * d;
                rem_len -= k;
                break; /* move to next digit */
            }
        }
    }

    buf[pos] = '\0';
    return str_to_seq(buf);
}
