/*
 * A282354  Positive j such that d(j) = d(j + 2*d(j)), where d(j) is the
 *          number of divisors of j.
 * https://oeis.org/A282354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9594 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of positive integers j such that
   d(j) = d(j + 2*d(j)), where d(n) is the number of divisors of n. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* static state for memoisation */
static seq_t *terms = NULL;    /* array of found terms       */
static size_t terms_cap = 0;   /* allocated capacity        */
static size_t terms_sz = 0;    /* number of stored terms    */
static idx_t  last_checked = 0;/* last j that has been examined */

/* Grow the terms array to hold at least 'needed' elements. */
static void ensure_terms_cap(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    terms = seq_realloc(terms, terms_cap, new_cap);
    terms_cap = new_cap;
}

/* Add a new term to the static array. */
static void add_term(seq_t val)
{
    ensure_terms_cap(terms_sz + 1);
    terms[terms_sz++] = (seq_t)val;
}

/* Main function exposed to the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset == 1, guard for misuse */

    /* initialise on first call */
    if (terms == NULL) {
        terms_cap = 8;
        terms = seq_realloc(NULL, 0, terms_cap);
        terms_sz = 0;
        last_checked = 0;
    }

    /* generate terms until we have at least n of them */
    while (terms_sz < (size_t)n) {
        ++last_checked;                      /* next j to examine */

        /* compute divisor counts */
        seq_t d_j   = num_divisors((uint64_t)last_checked);
        idx_t d_j_idx = (idx_t)d_j;
        idx_t target  = last_checked + 2 * d_j_idx;

        seq_t d_target = num_divisors((uint64_t)target);

        if (d_j == d_target)
            add_term((seq_t)last_checked);
    }

    return terms[n - 1];
}
