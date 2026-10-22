/*
 * A329367  Numbers whose binary expansion, without the most significant
 *          digit, is not a necklace.
 * https://oeis.org/A329367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   bitlen_seq_t, is_necklace
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non-formula: a(n) is the n-th positive integer whose binary representation,
   after removing its most significant bit, is not a necklace. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/is_necklace.h"

static seq_t *terms = NULL;
static size_t term_count = 0;   /* number of terms stored */
static size_t term_cap   = 0;   /* capacity of the array */
static seq_t next_candidate = 1;  /* next integer to test */

/* Ensure the terms array can hold at least 'need' elements. */
static void ensure_terms_capacity(size_t need)
{
    if (term_cap >= need) return;
    size_t new_cap = term_cap ? term_cap * 2 : 64;
    if (new_cap < need) new_cap = need;
    seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_terms) abort();   /* allocation failed */
    terms = new_terms;
    term_cap = new_cap;
}

/* Return true if 'm' satisfies the property “binary string without its
   most significant bit is NOT a necklace”. */
static bool is_valid_value(seq_t m)
{
    unsigned lenbits = bitlen_seq_t(m);
    if (lenbits <= 1) return false;   /* empty suffix is not considered */
    unsigned k = lenbits - 1;          /* length of the suffix */
    seq_t topbit = ((seq_t)1 << k);
    seq_t suffix = m - topbit;         /* remove the most significant bit */
    return !is_necklace(suffix, k);
}

/* Generate terms until we have at least 'n' terms stored. */
static void generate_terms(idx_t n)
{
    while ((idx_t)term_count < n) {
        if (is_valid_value(next_candidate))
            ensure_terms_capacity(term_count + 1), terms[term_count++] = next_candidate;
        ++next_candidate;
    }
}

/* The required function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n==0, return 0 */
    generate_terms(n);
    return terms[n - 1];
}
