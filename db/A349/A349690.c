/*
 * A349690  Numbers k such that the continued fraction of the abundancy index
 *          of k contains distinct elements.
 * https://oeis.org/A349690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 3887, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code computes A349690: the numbers k such that the continued
   fraction of the abundancy index sigma(k)/k has all distinct elements.
   It generates terms on demand and memoises them. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

static seq_t *terms = NULL;   /* cached sequence values          */
static size_t terms_len = 0;  /* number of terms stored          */
static size_t terms_cap = 0;  /* allocated capacity of terms     */
static idx_t next_k = 1;      /* smallest integer k not yet tested */

/* Ensure the cache can hold at least 'need' terms. */
static void grow_terms(size_t need)
{
    if (terms_cap >= need)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    terms = tmp;
    terms_cap = new_cap;
}

/* Return true iff the continued fraction of num/den has
   all distinct coefficients (num, den > 0).  The
   coefficients are derived by the Euclidean algorithm. */
static bool distinct_cf(seq_t num, seq_t den)
{
    /* Reduce the fraction first. */
    seq_t g = gcd(num, den);
    num = num / g;
    den = den / g;

    seq_t cf[128];
    size_t cf_len = 0;

    seq_t n = num, d = den;
    while (d != (seq_t)0) {
        seq_t t = n / d;          /* next coefficient */
        for (size_t i = 0; i < cf_len; ++i)
            if (t == cf[i])
                return false;     /* duplicate found */
        cf[cf_len++] = t;
        seq_t rem = n - t * d;    /* remainder */
        n = d;
        d = rem;
    }
    return true;
}

/* Return the n‑th element (1‑based) of A349690. */
seq_t a(idx_t n)
{
    while (terms_len < (size_t)n) {
        seq_t sigma = sigma_of(next_k);              /* sigma(k)   */
        if (distinct_cf(sigma, (seq_t)next_k)) {      /* distinct CF? */
            grow_terms(terms_len + 1);
            terms[terms_len++] = next_k;
        }
        ++next_k;
    }
    return terms[(size_t)n - 1];
}
