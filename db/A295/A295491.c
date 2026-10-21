/*
 * A295491  Numbers that have exactly eight representations as a sum of six
 *          nonnegative squares.
 * https://oeis.org/A295491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   The sequence a(n) is the list of integers that have exactly
 *            eight representations as a sum of six nonnegative squares
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#define LIMIT 5408

seq_t a(idx_t n) {
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static bool ready = false;
    if (!ready) {
        /* Find the largest square needed. */
        idx_t maxroot = 0;
        while ((maxroot + 1) * (maxroot + 1) <= LIMIT)
            ++maxroot;

        /* dp[k][s] = number of unordered multisets of k squares that sum to s. */
        static uint64_t dp[7][LIMIT + 1];
        for (idx_t k = 0; k <= 6; ++k)
            for (idx_t s = 0; s <= LIMIT; ++s)
                dp[k][s] = 0;
        dp[0][0] = 1;

        for (idx_t i = 0; i <= maxroot; ++i) {
            idx_t sq = i * i;
            for (idx_t k = 0; k < 6; ++k) {
                for (idx_t s = 0; s <= LIMIT - sq; ++s) {
                    uint64_t v = dp[k][s];
                    if (v) dp[k + 1][s + sq] += v;
                }
            }
        }

        /* Collect all n with exactly eight representations. */
        size_t cap = 64;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        terms_len = 0;
        for (idx_t val = 0; val <= LIMIT; ++val) {
            if (dp[6][val] == 8) {
                if (terms_len == cap) {
                    cap <<= 1;
                    terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
                }
                terms[terms_len++] = (seq_t)val;
            }
        }
        ready = true;
    }
    if (n == 0 || n > terms_len)
        return (seq_t)0;
    return terms[n - 1];
}
