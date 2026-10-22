/*
 * A370866  Positive integers m such that c(0) >= c(1) <= c(2), where c(k) =
 *          number of k's in the ternary representation of m.
 * https://oeis.org/A370866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer m such that * in the ternary
 *            representation of m the count of digit 1 * is less than or equal
 *            to the counts of digits 0 and 2. * Computed by enumerating
 *            integers and checking the condition
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   check_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/check_value.h"

/* ------------------------------------- */
/*   Memoised sequence storage           */
/* ------------------------------------- */
static seq_t *terms = NULL;        /* array of found terms   */
static size_t terms_size = 0;     /* number of terms stored */
static size_t terms_capacity = 0; /* current allocated capacity */
static seq_t next_candidate = 1;   /* next integer to test   */

/* Grow the terms array when needed. */
static void grow_terms(void)
{
    size_t new_cap = terms_capacity ? terms_capacity * 2 : 128;
    seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (new_ptr == NULL)
    {
        /* In the unlikely event of allocation failure, exit */
        abort();
    }
    terms = new_ptr;
    terms_capacity = new_cap;
}

/* Compute terms up to at least the nth term. */
static void ensure_terms(idx_t n)
{
    /* n is 1-based. */
    while ((size_t)n > terms_size)
    {
        /* Find next candidate that satisfies the property. */
        while (!check_value(next_candidate))
        {
            next_candidate++;
        }
        /* Store this candidate. */
        if (terms_size == terms_capacity)
            grow_terms();
        terms[terms_size++] = next_candidate;
        next_candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but return 0 for safety */
    ensure_terms(n);
    return terms[n - 1];
}
