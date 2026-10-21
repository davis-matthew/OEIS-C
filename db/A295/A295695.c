/*
 * A295695  Numbers that have exactly five representations as a sum of six
 *          positive squares.
 * https://oeis.org/A295695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Count unordered ways to write n as a sum of six positive
   squares.  All n for which the count equals exactly five are stored
   in an array; a(idx_t n) returns the n‑th such number (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static state for memoised sequence */
static seq_t *terms = NULL;          /* array of sequence terms   */
static size_t terms_len = 0;          /* number of terms stored    */
static size_t terms_cap = 0;          /* capacity of terms array   */
static size_t current_limit = 0;      /* highest sum currently processed */
static bool initialized = false;      /* flag that sequence is built */

/* Append a value to the terms array, growing capacity as needed */
static void append_term(seq_t val)
{
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = val;
}

/* Build all terms up to at least the required index (if possible).  An
   exponential back‑off strategy is used: start with an initial limit
   and double it until enough terms are found.  The DP is recomputed
   from scratch each time a larger limit is required.  The counts are
   capped at 6 to avoid overflow; a value of exactly 5 indicates a
   valid term. */
static void build_terms(idx_t need_index)
{
    if (initialized && terms_len >= need_index)
        return;

    /* Start with a modest limit and grow until enough terms are found */
    size_t limit = current_limit ? current_limit : 2000;   /* initial guess */

    while (1) {
        /* free any previously allocated terms */
        free(terms);
        terms = NULL;
        terms_len = 0;
        terms_cap = 0;

        /* Allocate DP table: 7 rows (used 0..6) each of length limit+1,
           stored as a flat array of uint8_t (capped counts). */
        size_t stride = limit + 1;
        uint8_t *dp = (uint8_t *)calloc(7 * stride, sizeof(uint8_t));
        if (!dp) exit(1);   /* out of memory */

        dp[0] = 1;  /* zero squares sum to 0 in one way */

        /* Generate all square values up to the limit */
        size_t max_base = (size_t)__builtin_sqrt((long double)limit);
        for (size_t base = 1; base <= max_base; ++base) {
            size_t v = base * base;
            /* Update DP for this square value */
            for (int used = 1; used <= 6; ++used) {
                uint8_t *dp_used = dp + used * stride;
                uint8_t *dp_prev = dp + (used - 1) * stride;
                for (size_t sum = v; sum <= limit; ++sum) {
                    uint8_t add = dp_prev[sum - v];
                    if (!add) continue;          /* nothing to add */
                    uint8_t cur = dp_used[sum] + add;
                    if (cur > 6) cur = 6;        /* cap at 6 for safety */
                    dp_used[sum] = cur;
                }
            }
        }

        /* Collect all sums whose count equals exactly 5 */
        for (size_t sum = 1; sum <= limit; ++sum) {
            if (dp[6 * stride + sum] == 5)
                append_term((seq_t)sum);
        }

        free(dp);          /* DP no longer needed */

        /* If we have enough terms, keep this limit and finish */
        if (terms_len >= need_index) {
            current_limit = limit;
            initialized = true;
            break;
        }

        /* Otherwise, double the limit and try again */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined, but safe to return 0 */

    /* Build terms if not yet available or insufficient for this index */
    build_terms(n);

    if (n - 1 < terms_len)
        return terms[n - 1];
    else
        return 0;           /* index beyond the finite sequence */
}
