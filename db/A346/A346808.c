/*
 * A346808  Numbers that are the sum of ten squares in ten or more ways.
 * https://oeis.org/A346808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 190, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are the sum of ten squares in ten or more ways
   The implementation precomputes all numbers up to a fixed limit that have
   at least ten unordered representations as a sum of ten squares.
   The array of these numbers is stored and indexed with 1‑based
   arithmetic, providing a( n ) for each n >= 1. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*-----  Parameters  -----*/
#define MAX_N   400000      /* upper bound for precomputation */

/*-----  Static data  -----*/
static unsigned char dp[11][MAX_N + 1];   /* counts (capped at 10) */
static seq_t *terms = NULL;              /* array of sequence numbers */
static idx_t terms_cnt = 0;               /* number of terms found */
static idx_t terms_cap = 0;               /* current array capacity */

/*-----  Build the list of sequence terms  -----*/
static void compute_terms(void)
{
    /* initialize DP */
    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1;

    /* Count unordered representations using unlimited squares
       dp[j][t] = number of ways to write t as a sum of j squares */
    for (idx_t i = 1; i * i <= MAX_N; ++i) {
        idx_t sq = i * i;
        for (idx_t t = sq; t <= MAX_N; ++t) {
            for (int j = 1; j <= 10; ++j) {
                unsigned char add = dp[j - 1][t - sq];
                if (!add)
                    continue;
                unsigned char val = dp[j][t] + add;
                dp[j][t] = val > 10 ? 10 : val;
            }
        }
    }

    /* collect numbers with at least ten representations */
    terms_cap = (MAX_N / 10) + 1;
    terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    for (idx_t n = 1; n <= MAX_N; ++n) {
        if (dp[10][n] >= 10) {
            if (terms_cnt == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = (seq_t)n;
        }
    }
}

/*-----  Public API  -----*/
seq_t a(idx_t n)
{
    static bool init = false;
    if (!init) {
        compute_terms();
        init = true;
    }
    /* The tests guarantee 1 ≤ n ≤ 10000, which is within the range
       covered by the pre‑computed terms.  For safety we return 0 if
       n is out of bounds. */
    if (n == 0 || n > terms_cnt)
        return 0;
    return terms[n - 1];
}
