/*
 * A295743  Numbers that have exactly three representations of a sum of seven
 *          nonnegative squares.
 * https://oeis.org/A295743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This function returns the n-th value of OEIS sequence A295743: 
 * numbers that have exactly three unordered representations as a sum of
 * seven non‑negative squares.  The implementation precomputes all such
 * numbers up to 6000 (a safe upper bound implied by known theoretical
 * results) and memoises the list for repeated queries.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_COUNT 7          /* number of squares */
#define MAX_N 6000           /* safe upper bound for the sequence */

/* Memoised list of terms */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Helper to initialise the term list once */
static void init_terms(void)
{
    if (terms != NULL)
        return;                     /* already initialised */

    /* DP array: dp[ cnt * (MAX_N+1) + sum ] counts unordered ways */
    const size_t rows = MAX_COUNT + 1;            /* 0..7 */
    const size_t cols = MAX_N + 1;               /* 0..MAX_N */
    const size_t sz = rows * cols;

    unsigned int *dp = (unsigned int *)calloc(sz, sizeof(unsigned int));
    if (dp == NULL) {
        /* allocation failed – leave terms NULL */
        return;
    }

    dp[0 * cols + 0] = 1;     /* base: 0 squares sum to 0 */

    /* Pre‑compute the list of square values up to MAX_N */
    size_t max_root = 0;
    while ((max_root + 1) * (max_root + 1) <= MAX_N)
        ++max_root;
    size_t squares[80];    /* enough for roots up to 79 */
    size_t sqcnt = 0;
    for (size_t i = 0; i <= max_root; ++i)
        squares[sqcnt++] = i * i;

    /* Populate DP: classic unbounded knapsack style for unordered sums */
    for (size_t si = 0; si < sqcnt; ++si) {
        size_t sq = squares[si];
        for (size_t cnt = 1; cnt <= MAX_COUNT; ++cnt) {
            size_t base_curr = cnt * cols;
            size_t base_prev = (cnt - 1) * cols;
            for (size_t sum = sq; sum <= MAX_N; ++sum) {
                dp[base_curr + sum] += dp[base_prev + sum - sq];
            }
        }
    }

    /* Allocate initial term storage */
    terms_cap = 256;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (terms == NULL) {
        free(dp);
        return;
    }

    /* Collect all n with exactly 3 representations */
    for (size_t val = 1; val <= MAX_N; ++val) {
        if (dp[MAX_COUNT * cols + val] == 3U) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;            /* double capacity */
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (tmp == NULL) {          /* allocation failed */
                    free(terms);
                    terms = NULL;
                    free(dp);
                    return;
                }
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)val;
        }
    }

    free(dp);
}

/* PUBLIC interface required by the harness */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n < 1 */

    init_terms();                      /* ensure terms are ready */

    if (terms == NULL || (size_t)n > terms_len)
        return 0;                      /* out of range */

    return terms[n - 1];
}
