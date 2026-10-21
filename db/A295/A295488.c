/*
 * A295488  Numbers that have exactly five representations as a sum of six
 *          nonnegative squares.
 * https://oeis.org/A295488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by counting unordered representations as a sum of six nonnegative squares.
   The function precomputes all such numbers up to a safe bound (10000) and stores them
   in increasing order.  Then a(n) returns the nth term (1‑based). */

#include <stdlib.h>
#include <stdint.h>

#define MAX_N 10000    /* safe limit > 5408, all values of the sequence lie below this */

seq_t a(idx_t n)
{
    /* static state to initialise only once */
    static seq_t *terms = NULL;      /* array of sequence terms */
    static idx_t term_count = 0;     /* number of terms stored */
    static int initialized = 0;

    if (!initialized) {
        /* dp[k][m] : number of unordered ways to write m as a sum of exactly k squares,
           using only squares 1^2,2^2,... .  k ranges 0..6, m ranges 0..MAX_N.  */
        static unsigned long long dp[7][MAX_N + 1];
        unsigned int s, square;
        unsigned int k, m;
        unsigned int rep;

        /* base case */
        dp[0][0] = 1ULL;

        /* count representations using dynamic programming (coin change style) */
        for (s = 1; s * s <= MAX_N; ++s) {
            square = s * s;
            for (k = 1; k <= 6; ++k) {
                for (m = square; m <= MAX_N; ++m) {
                    dp[k][m] += dp[k - 1][m - square];
                }
            }
        }

        /* first count how many numbers have exactly five representations */
        size_t cnt = 0;
        for (m = 1; m <= MAX_N; ++m) {
            unsigned long long total = 0;
            for (k = 1; k <= 6; ++k) {
                total += dp[k][m];
            }
            if (total == 5ULL) {
                ++cnt;
            }
        }

        if (cnt > 0) {
            /* allocate array for terms */
            terms = (seq_t *)malloc(cnt * sizeof(seq_t));
            if (!terms) return 0; /* allocation failure – return 0 */
            size_t pos = 0;
            for (m = 1; m <= MAX_N; ++m) {
                unsigned long long total = 0;
                for (k = 1; k <= 6; ++k) {
                    total += dp[k][m];
                }
                if (total == 5ULL) {
                    terms[pos++] = (seq_t)m;
                }
            }
            term_count = (idx_t)cnt;
        } else {
            term_count = 0;
        }
        initialized = 1;
    }

    /* bounds check: a(1) is the first term.  n==0 or n>term_count returns 0. */
    if (n == 0 || n > term_count) return 0;
    return terms[n - 1];
}
