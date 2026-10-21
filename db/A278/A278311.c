/*
 * A278311  Numbers n such that n-1 and n+1 have the same number of prime
 *          factors as n (with multiplicity).
 * https://oeis.org/A278311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1665, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A278311: numbers n such that Ω(n-1)=Ω(n)=Ω(n+1) */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/bigomega.h"

static seq_t *terms  = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;
static idx_t next_candidate = 2;

static void add_term(idx_t v)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = (seq_t)v;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for offset 0 */

    while (terms_len < (size_t)n) {
        seq_t om1 = bigomega(next_candidate - 1);
        seq_t om  = bigomega(next_candidate);
        seq_t op1 = bigomega(next_candidate + 1);
        if (om1 == om && om == op1) add_term(next_candidate);
        ++next_candidate;
    }
    return terms[n - 1];
}
