/*
 * A257636  Numbers n such that the base 10 reversals of n and n+1 are both
 *          prime.
 * https://oeis.org/A257636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1132, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8292 (2026-11-03)
 * Helpers:   is_prime, mem, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A257636 by progressive search: a(n) is the nth integer n
   such that rev(n) and rev(n+1) are both prime. */

#include <stdbool.h>   /* for bool */
#include <stddef.h>    /* for size_t, NULL */
#include <stdlib.h>    /* for malloc, realloc */

/* Utility functions provided by the build system */
#include "../utils/is_prime.h"
#include "../utils/reverse_digits_idx.h"
#include "../utils/mem.h"

/* Static cache holding already found terms. */
static seq_t  *terms    = NULL;   /* array of sequence values */
static size_t  terms_len  = 0;    /* number of terms stored so far */
static size_t  terms_cap  = 0;    /* current capacity of the array */
static idx_t   cand_idx   = 1;    /* next candidate to test (1‑based) */

/* Ensure that at least `target` terms are stored in the cache. */
static void ensure_terms(size_t target)
{
    /* Lazy allocation of the array on first use. */
    if (!terms) {
        terms_cap = 16;                                 /* initial capacity */
        terms = mem_new(seq_t, terms_cap);
        if (!terms) { return; }                         /* avoid crash if malloc fails */
    }

    /* Scan consecutive candidates until we have enough terms. */
    for (; terms_len < target; ++cand_idx) {
        idx_t cur = cand_idx;
        /* Test the reversal of cur and cur+1 for primality. */
        if (is_prime(reverse_digits_idx(cur)) && is_prime(reverse_digits_idx(cur + 1))) {
            /* Grow the array if necessary. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap * 2;
                seq_t *tmp = mem_resize(seq_t, terms, new_cap);
                if (!tmp) { return; }                    /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            /* Store the found term. Convert cur to seq_t (safe for small values). */
            terms[terms_len++] = (seq_t)cur;
        }
    }
}

/* Return the `n`‑th term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, undefined for 0 */
    /* Ensure cache covers the requested index. */
    ensure_terms((size_t)n);
    /* The array is zero‑based, so index n-1. */
    return terms[n - 1];
}
