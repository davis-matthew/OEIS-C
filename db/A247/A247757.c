/*
 * A247757  Numbers in decimal representation, such that in Italian and
 *          Portuguese their digits are in alphabetic order.
 * https://oeis.org/A247757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, word
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 2368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n‑th smallest decimal integer whose digits are alphabetically
 * ordered in Italian and Portuguese according to the order 5,2,9,8,4,6,7,3,1,0.
 * The function enumerates numbers by length and uses combinatorial counts to skip
 * over blocks of invalid numbers, yielding the desired term in O(length) time.
 */

#include <stdint.h>

/* Custom alphabetic order of digits:
 * 5 < 2 < 9 < 8 < 4 < 6 < 7 < 3 < 1 < 0
 * ord[d] gives the position of digit d in this order. */
static const int ord[10] = {9,8,1,7,4,0,5,6,3,2};

/* Pascal triangle up to n = 20, enough for all needed lengths. */
static seq_t comb[21][21];
static int comb_init = 0;

static void init_comb(void)
{
    if (comb_init) return;
    comb_init = 1;
    for (int n = 0; n <= 20; ++n) {
        for (int k = 0; k <= n; ++k) {
            if (k == 0 || k == n) comb[n][k] = 1;
            else comb[n][k] = comb[n-1][k-1] + comb[n-1][k];
        }
    }
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1. */
    init_comb();

    /* Find the length of the desired number. */
    seq_t cumulative_before = 0;
    idx_t len = 0;
    for (idx_t L = 1; ; ++L) {
        seq_t countL;
        if (L == 1) {
            countL = 10;                /* 0–9 */
        } else {
            countL = comb[L+9][L] - 1;  /* exclude the all‑zero string */
        }
        if ((seq_t)n <= cumulative_before + countL) {
            len = L;
            break;
        }
        cumulative_before += countL;
    }

    /* Index within the chosen length (1‑based). */
    seq_t target = (seq_t)n - cumulative_before;

    seq_t number = 0;
    int prev_ord = -1;  /* undefined for the first digit */

    for (idx_t pos = 0; pos < len; ++pos) {
        int start = (pos == 0 && len > 1) ? 1 : 0;  /* no leading zero for len>1 */

        for (int d = start; d <= 9; ++d) {
            if (pos > 0 && ord[d] < prev_ord) continue;  /* maintain non‑decreasing order */

            idx_t rem = len - pos - 1;
            seq_t cnt;
            if (rem == 0) {
                cnt = 1;
            } else {
                int k = 10 - ord[d];                /* digits ≥ d in the custom order */
                cnt = comb[k + rem - 1][rem];
            }

            if (target > cnt) {
                target -= cnt;
            } else {
                /* Choose this digit and move to the next position. */
                number = number * 10 + d;
                prev_ord = ord[d];
                break;
            }
        }
    }

    return number;
}
