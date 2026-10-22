/*
 * A375007  Numbers t which satisfy the equation: t mod k = floor((t - k)/k)
 *          mod k (1 <= k <= t) only for k = 1 and t.
 * https://oeis.org/A375007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 38, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A375007
 *
 * Definition: t satisfies t mod k = floor((t-k)/k) mod k for
 * 1 ≤ k ≤ t only for k = 1 and k = t.
 * A sieve is used: for each k ≥ 2 and each integer q ≥ 1,
 * all t of the form
 *
 *     t = k*q + (q-1) mod k
 *
 * satisfy the equation for that k (except t = k itself).
 * We mark all such t and keep only the numbers that never get
 * marked.  The resulting list of numbers (1‑based) is the sequence.
 *
 * The implementation builds a sieve up to an increasing limit until
 * the required number of terms is found.  It stores all found
 * terms in a dynamic array and returns the n‑th term.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static idx_t  sieve_limit   = 0;   /* current sieve upper bound          */
static uint8_t *valid_table  = NULL;/* 0 = not a valid term, 1 = valid   */
static seq_t *terms         = NULL;/* array of found sequence values      */
static size_t  terms_len     = 0;   /* number of terms currently stored    */
static size_t  terms_cap     = 0;   /* current capacity of 'terms'         */

/* Rebuild the sieve and the terms array up to 'limit'. */
static void rebuild(idx_t limit)
{
    /* Free any existing data. */
    free(valid_table);
    free(terms);

    /* Allocate the validity table. */
    valid_table = (uint8_t *)calloc((size_t)(limit + 1), 1);
    if (!valid_table) abort();
    valid_table[0] = 0;  /* 0 is never a term */

    /* Initially mark every t as possibly valid. */
    for (idx_t t = 1; t <= limit; ++t) valid_table[t] = 1;

    /* For each k, mark all t that satisfy the equation for that k. */
    for (idx_t k = 2; k <= limit; ++k) {
        idx_t rem = 0;                 /* (q-1) mod k  */
        for (idx_t q = 1; ; ++q) {
            /* stop when the next t would exceed the limit. */
            if (q > limit / k) break;  /* safety against overflow   */
            idx_t t = k * q + rem;
            if (t > limit) break;
            if (t != k)          /* exclude the trivial case k = t */
                valid_table[t] = 0;
            /* advance the remainder */
            ++rem;
            if (rem == k) rem = 0;
        }
    }

    /* Build the terms array from the validity table. */
    terms_cap = (size_t)limit + 1;      /* enough space for all valid t */
    terms = (seq_t *)calloc(terms_cap, sizeof(seq_t));
    if (!terms) abort();
    terms_len = 0;

    for (idx_t t = 1; t <= limit; ++t) {
        if (valid_table[t]) {
            if (terms_len >= terms_cap) {
                terms_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) abort();
                terms = tmp;
            }
            terms[terms_len++] = (seq_t)t;
        }
    }
    sieve_limit = limit;
}

/* Ensure that we have at least 'n' terms computed. */
static void ensure_terms(idx_t n)
{
    if (terms_len >= n) return;           /* already enough terms            */

    /* Initial guess for the limit if nothing built yet. */
    idx_t limit = sieve_limit;
    if (limit == 0) limit = 10;           /* at least a few entries          */

    /* Rough heuristic: 2000 times n plus some buffer. */
    idx_t target = n * 2000 + 100;
    if (target > limit) limit = target;

    /* Keep doubling until we have enough terms. */
    while (terms_len < n) {
        rebuild(limit);
        /* If still insufficient, enlarge the limit. */
        if (terms_len < n) {
            limit *= 2;
            if (limit == 0) limit = 1ULL << 62; /* avoid overflow */
        }
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) abort();   /* sequence is 1‑based */
    ensure_terms(n);
    return terms[n - 1];
}
