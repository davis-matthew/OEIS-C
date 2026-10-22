/*
 * A355985  Primes whose reversal is a multiple of 16.
 * https://oeis.org/A355985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime_big, reverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth prime p such that reverse(p) is a multiple of 16. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/reverse.h"        /* provides seq_t reverse(seq_t) */
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t) */

static seq_t *terms = NULL;          /* cached terms */
static size_t term_count = 0;        /* how many terms are stored */
static size_t term_cap = 0;          /* current capacity of terms array */
static idx_t next_candidate = 2;     /* integer value of the next number to test */

/* Grow the terms array when needed. */
static void grow_terms(void)
{
    if (term_cap == 0) {
        term_cap = 16;
        terms = (seq_t *)malloc(term_cap * sizeof(seq_t));
    } else {
        term_cap <<= 1;
        terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
    }
}

/* Generate and store terms until we have at least n of them. */
static void ensure_terms(idx_t n)
{
    while (term_count < n) {
        /* candidate number expressed as seq_t */
        seq_t cand = (seq_t)next_candidate++;
        /* skip numbers that overflow seq_t; they cannot be represented */
        if (cand == 0) break;

        if (!is_prime_big(cand))
            continue;

        seq_t rev = reverse(cand);
        if (rev % (seq_t)16 != 0)
            continue;

        if (term_count == term_cap)
            grow_terms();

        terms[term_count++] = cand;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;      /* undefined for n = 0 (offset is 1) */
    ensure_terms(n);
    return terms[n - 1];
}
