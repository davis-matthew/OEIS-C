/*
 * A322840  Positive integers n with fewer prime factors (counted with
 *          multiplicity) than n + 1.
 * https://oeis.org/A322840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A322840: a(n) = the nth positive integer m such that
   bigomega(m) < bigomega(m+1)   (indexing starts at 1). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/count_prime_factors.h"

/* Static cache for the sequence */
static seq_t *terms = NULL;      /* array of found terms          */
static size_t terms_len = 0;     /* number of terms stored        */
static size_t terms_cap = 0;     /* current allocated capacity    */
static idx_t   checked  = 0;     /* largest integer examined so far */

/* Extend the cache to at least 'need' elements.
   Uses the standard realloc; the cast is safe for both C and C++. */
static void grow_terms(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* Append a new term to the cache */
static void add_term(seq_t val)
{
    grow_terms(terms_len + 1);
    terms[terms_len++] = val;
}

/* Return the nth term of the sequence (n >= 1).  Behaviour
   for n == 0 is undefined; we return 0 just to satisfy the
   function signature. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined for offset 0 */

    while (terms_len < (size_t)n) {
        /* examine the next integer */
        ++checked;
        seq_t omega_m     = count_prime_factors((seq_t)checked);
        seq_t omega_mplus = count_prime_factors((seq_t)(checked + 1));
        if (omega_m < omega_mplus)
            add_term(checked);
    }
    return terms[n - 1];
}
