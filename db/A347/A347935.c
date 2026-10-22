/*
 * A347935  Numbers k such that A187795(k) > 2*k.
 * https://oeis.org/A347935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 1448, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A347935:
   Numbers k such that the sum of all abundant divisors of k
   exceeds 2*k.  The function returns the nth term of the
   sequence (1‑based).  It uses a sieve to precompute sigma, build
   the abundant set, sum abundant divisors for each n, and
   stores all terms in increasing order.  The sieve limit is
   expanded dynamically until it contains at least n terms. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *sigma = NULL;          /* sum of divisors */
static unsigned char *abundant = NULL;  /* bool flag per number */
static seq_t *sum_abundant = NULL;  /* sum of abundant divisors */
static seq_t *terms = NULL;         /* sequence terms */
static size_t term_cap = 0;          /* allocated capacity */
static size_t term_cnt = 0;          /* number of terms stored */
static idx_t current_limit = 0;      /* largest n processed by arrays */

/* Ensure terms array can hold at least 'needed' elements */
static void ensure_terms_capacity(size_t needed)
{
    if (needed <= term_cap) return;
    size_t new_cap = term_cap ? term_cap : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp && new_cap) { /* allocation failure – abort */
        abort();
    }
    terms = tmp;
    term_cap = new_cap;
}

/* Recompute all auxiliary arrays up to 'limit'
   and rebuild the complete terms list.  Any previously
   allocated memory smaller than 'limit+1' is expanded. */
static void recompute_upto(idx_t limit)
{
    /* Allocate or resize storage */
    sigma = (seq_t *)realloc(sigma, (limit + 1) * sizeof(seq_t));
    abundant = (unsigned char *)realloc(abundant, (limit + 1) * sizeof(unsigned char));
    sum_abundant = (seq_t *)realloc(sum_abundant, (limit + 1) * sizeof(seq_t));
    if (!sigma || !abundant || !sum_abundant) abort();

    /* Initialise sigma array */
    for (idx_t i = 0; i <= limit; ++i) sigma[i] = 0;

    /* Compute sigma[n] = sum of divisors of n */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            sigma[j] += (seq_t)i;
        }
    }

    /* Build abundant flag array */
    for (idx_t i = 1; i <= limit; ++i) {
        abundant[i] = sigma[i] > (seq_t)(2u * (seq_t)i);
    }

    /* Initialise sum_abundant array */
    for (idx_t i = 0; i <= limit; ++i) sum_abundant[i] = 0;

    /* For each abundant d, add d to all multiples */
    for (idx_t d = 1; d <= limit; ++d) {
        if (abundant[d]) {
            for (idx_t j = d; j <= limit; j += d) {
                sum_abundant[j] += (seq_t)d;
            }
        }
    }

    /* Build terms list */
    term_cnt = 0;
    ensure_terms_capacity(limit / 2 + 10);
    for (idx_t n = 1; n <= limit; ++n) {
        if (sum_abundant[n] > (seq_t)(2u * (seq_t)n)) {
            if (term_cnt >= term_cap) ensure_terms_capacity(term_cnt + 1);
            terms[term_cnt++] = (seq_t)n;
        }
    }

    current_limit = limit;
}

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined but safe fallback */

    /* Ensure we have at least n terms */
    if (term_cnt < n || current_limit == 0) {
        /* Start with some reasonable limit */
        idx_t limit = current_limit ? current_limit * 2 : 1000;
        while (term_cnt < n) {
            recompute_upto(limit);
            /* Grow limit if still not enough terms */
            limit <<= 1;
        }
    }

    /* Indexing is 1‑based in the OEIS specification */
    return terms[n - 1];
}
