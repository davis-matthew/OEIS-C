/*
 * A345483  Numbers that are the sum of seven squares in six or more ways.
 * https://oeis.org/A345483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 194, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation:
   a(n) is the n‑th integer that can be written as a sum of 7 integer squares
   in at least 6 distinct unordered ways.
   The function builds a small dynamic programming table once and reuses it
   for all subsequent calls.  Data are stored in unsigned short because
   only the fact that a count reaches 6 (or 65535) matters.  The table is
   large enough to provide at least the first 10 000 terms.  If the requested
   index exceeds the pre‑computed list, the function returns 0. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---------- static data ------------------------------------ */
    static bool initialized = false;
    /* pre‑compute up to MAX_N squares; large enough for ~10⁴ terms. */
    static const idx_t MAX_N = 500000;

    static unsigned short *dp[8];       /* dp[rep][sum] 0‑≤rep≤7   */
    static idx_t *terms;                /* list of qualifying numbers */
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    if (!initialized) {
        /* ----- allocate DP tables ------------------------------------ */
        for (int r = 0; r < 8; ++r) {
            dp[r] = (unsigned short *)calloc((size_t)MAX_N + 1,
                                             sizeof(unsigned short));
            if (!dp[r]) { abort(); }
        }
        dp[0][0] = 1;  /* one way to write 0 as sum of 0 squares */

        /* ----- dynamic programming over squares 1²,2²,…---------- */
        for (idx_t root = 1; root * root <= MAX_N; ++root) {
            idx_t sq = root * root;
            for (int rep = 0; rep < 7; ++rep) {           /* build rep+1 */
                unsigned short *prev = dp[rep];
                unsigned short *curr = dp[rep + 1];
                for (idx_t s = sq; s <= MAX_N; ++s) {
                    unsigned short inc = prev[s - sq];
                    if (!inc) continue;
                    unsigned int sum = (unsigned int)curr[s] + inc;
                    if (sum > 0xFFFFu) sum = 0xFFFFu;
                    curr[s] = (unsigned short)sum;
                }
            }
        }

        /* ----- collect numbers with >=6 7‑squares representations */
        terms_cap = (size_t)MAX_N + 1;
        terms = (idx_t *)malloc(terms_cap * sizeof(idx_t));
        if (!terms) { abort(); }

        for (idx_t s = 1; s <= MAX_N; ++s) {
            if (dp[7][s] >= 6) {
                terms[terms_len++] = s;
            }
        }

        initialized = true;
    }

    /* ---------- answer lookup --------------------------------- */
    if (n == 0) return 0;            /* undefined; guard against 0 */
    if (n - 1 < terms_len) {
        return (seq_t)terms[n - 1];
    }
    /* If the requested index exceeds our pre‑computed list, return 0. */
    return 0;
}
