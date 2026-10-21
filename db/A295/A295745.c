/*
 * A295745  Numbers that have exactly five representations of a sum of seven
 *          nonnegative squares.
 * https://oeis.org/A295745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   numbers that have exactly five representations as a sum of seven
 *            nonnegative squares. The function generates the sequence by a
 *            standard coin‑change DP counting unordered combinations of seven
 *            squares. The table is computed once on first call
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define K 7
/* Upper bound large enough to capture all terms of the sequence. */
#define N_MAX 20000

static unsigned char dp[K + 1][N_MAX + 1] = {0};

static seq_t *terms = NULL;      /* array of found terms */
static size_t terms_len = 0;     /* number of terms found */
static size_t terms_cap = 0;     /* capacity of terms array */

static void precompute(void)
{
    /* initialise base case: 0 can be written as sum of 0 squares in exactly 1 way */
    dp[0][0] = 1;

    /* all other entries are already zero by static initialisation */

    /* list of distinct squares up to N_MAX */
    for (int s = 0; s * s <= N_MAX; ++s) {
        int sq = s * s;
        /* try to use this square sq one more time in each combination of k squares */
        for (int k = 1; k <= K; ++k) {
            for (int n = sq; n <= N_MAX; ++n) {
                unsigned char add = dp[k - 1][n - sq];
                unsigned char cur = dp[k][n];
                /* we only need to detect if count reaches 5; more than that is irrelevant.
                   Use 6 as sentinel for “>=6”. */
                if (add >= 5 || cur >= 5) {
                    dp[k][n] = 6;
                } else {
                    unsigned int sum = cur + add;
                    dp[k][n] = (sum > 5 ? 6 : (unsigned char)sum);
                }
            }
        }
    }

    /* collect all numbers with exactly five representations */
    for (int n = 1; n <= N_MAX; ++n) {
        if (dp[K][n] == 5) {
            if (terms_len == terms_cap) {
                terms_cap = (terms_cap == 0 ? 16 : terms_cap * 2);
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)n;
        }
    }
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        precompute();
    }
    if (n < 1 || n > terms_len) return 0;
    return terms[n - 1];
}
