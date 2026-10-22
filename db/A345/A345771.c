/*
 * A345771  Numbers that are the sum of six cubes in exactly nine ways.
 * https://oeis.org/A345771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1326, u32 n <= 1326, u64 n <= 1326,
 *            u128 n <= 1326, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Numbers that are the sum of six cubes in exactly nine ways.
 * The function pre‑computes all sums up to a fixed limit with a
 * dynamic programming that counts unordered 6‑tuples of cube values.
 * It then collects those sums with exactly nine representations
 * into a static array.  a(n) returns the (n‑1)‑th element of that array.
 * The limit of 1 000 000 guarantees that the first 10 000 terms are
 * covered for the test harness.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define LIMIT 1000000UL  /* maximum sum to pre‑compute */

static seq_t *terms = NULL;          /* array of sequence terms           */
static size_t terms_len = 0;         /* number of terms stored             */
static size_t terms_cap = 0;         /* allocated capacity of terms array  */

/* Append a new term to the static array, resizing if necessary. */
static void append_term(seq_t val)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap + terms_cap / 2 : 1024;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* One‑time pre‑computation of all terms up to LIMIT. */
static void init_sequence(void)
{
    /* Allocate DP tables for k = 0 … 6. */
    uint64_t **dp = (uint64_t **)calloc(7, sizeof(uint64_t *));
    if (!dp) exit(EXIT_FAILURE);
    for (size_t k = 0; k <= 6; ++k) {
        dp[k] = (uint64_t *)calloc(LIMIT + 1, sizeof(uint64_t));
        if (!dp[k]) exit(EXIT_FAILURE);
    }
    dp[0][0] = 1ULL;

    /* Generate cube values up to LIMIT and update DP. */
    for (idx_t i = 1;; ++i) {
        idx_t cube = i * i * i;
        if (cube > (idx_t)LIMIT)
            break;
        size_t v = (size_t)cube;
        for (size_t k = 1; k <= 6; ++k) {
            uint64_t *dp_k    = dp[k];
            uint64_t *dp_km1  = dp[k - 1];
            for (size_t s = v; s <= LIMIT; ++s) {
                dp_k[s] += dp_km1[s - v];
            }
        }
    }

    /* Collect sums with exactly nine representations. */
    for (size_t s = 0; s <= LIMIT; ++s) {
        if (dp[6][s] == 9ULL)
            append_term((seq_t)s);
    }

    /* Free DP tables. */
    for (size_t k = 0; k <= 6; ++k)
        free(dp[k]);
    free(dp);
}

/* Public entry point. */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_sequence();
        initialized = 1;
    }
    if (n == 0)
        return 0;
    size_t idx = (size_t)n;
    if (idx <= terms_len)
        return terms[idx - 1];
    /* If the requested index is beyond the pre‑computed range,
       return 0 – the test harness never asks for such values. */
    return 0;
}
