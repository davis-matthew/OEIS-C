/*
 * A352464  Positive numbers k whose decimal expansion ends in the product of
 *          digits of k.
 * https://oeis.org/A352464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 6660, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, digit_product, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A352464:
   a(n) = the nth positive integer whose decimal representation ends
   with the product of its decimal digits. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Static state for memoization */
static seq_t *terms = NULL;      /* array of found terms */
static size_t terms_len = 0;     /* number of terms stored */
static size_t terms_cap = 0;     /* capacity of the array */
static idx_t next_candidate = 1; /* next integer to test */

/* Ensure the terms array can hold at least 'needed' elements */
static void ensure_terms_cap(size_t needed)
{
    if (needed <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms = tmp;
    terms_cap = new_cap;
}

/* Predicate: does k end with the product of its digits? */
static bool is_good(seq_t k)
{
    seq_t prod = digit_product(k);     /* product of digits */
    int d = digit_len(prod);           /* number of decimal digits of prod */
    if (prod == 0 || d == 0)
        d = 1;                         /* ensure at least one digit for 0 */
    seq_t mod = pow10((idx_t)d);        /* 10^d as seq_t */
    return (k % mod) == prod;          /* suffix equals product */
}

/* Main function: return the nth term (1-based) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0; guard */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        seq_t cand = (seq_t)next_candidate;
        if (is_good(cand)) {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = cand;
        }
        next_candidate++;
    }
    return terms[n - 1];
}
