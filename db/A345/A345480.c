/*
 * A345480  Numbers that are the sum of seven squares in three or more ways.
 * https://oeis.org/A345480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 218, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: DP to count unordered representations of n as a sum of 7
   positive squares; collect numbers that have at least 3 such representations. */

#include <stdlib.h>
#include <stdint.h>

/* Dynamic programming table: counts of representations up to 3.
   dp_counts[k][s] = number of unordered ways to write s as the sum of exactly k
   positive squares, capped at 3. */
static uint8_t **dp_counts;
static seq_t *terms;          /* collected terms with >=3 representations */
static size_t terms_len;
static int built = 0;

/* Build DP once and collect the first 10 000 terms. */
static void build_table(void)
{
    if (built) return;
    built = 1;

    const idx_t MAXM = 200000;   /* window large enough for 10⁴ terms          */
    const int K = 7;             /* number of squares                         */

    /* Allocate counts table. */
    dp_counts = (uint8_t**)malloc((K + 1) * sizeof(uint8_t*));
    for (int k = 0; k <= K; ++k)
        dp_counts[k] = (uint8_t*)calloc((size_t)(MAXM + 1), sizeof(uint8_t));

    dp_counts[0][0] = 1;         /* one way to write 0 with 0 squares         */

    /* Populate table using coin‑change style DP (unordered sums). */
    for (idx_t val = 1; ; ++val) {
        idx_t sq = val * val;
        if (sq > MAXM) break;
        for (int k = 1; k <= K; ++k) {
            for (idx_t s = sq; s <= MAXM; ++s) {
                uint8_t add = dp_counts[k - 1][(size_t)(s - sq)];
                if (add == 0) continue;
                uint8_t newv = dp_counts[k][(size_t)s] + add;
                if (newv > 3) newv = 3;
                dp_counts[k][(size_t)s] = newv;
            }
        }
    }

    /* Collect numbers with at least three representations. */
    size_t cap = 10000;
    terms = (seq_t*)malloc(cap * sizeof(seq_t));
    terms_len = 0;

    for (idx_t s = 1; s <= MAXM; ++s) {
        if (dp_counts[K][(size_t)s] >= 3) {
            if (terms_len == cap) {
                cap *= 2;
                terms = (seq_t*)realloc(terms, cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)s;
            if (terms_len == 10000) break;
        }
    }

    /* Free DP table, not needed after this point. */
    for (int k = 0; k <= K; ++k) free(dp_counts[k]);
    free(dp_counts);
    dp_counts = NULL;
}

/* Return the nth term of A345480 (indexing starts at 1). */
seq_t a(idx_t n)
{
    build_table();                     /* ensure the table has been built   */
    if (n < 1 || (size_t)n > terms_len)
        return 0;                      /* out of range – should not happen */
    return terms[n - 1];
}
