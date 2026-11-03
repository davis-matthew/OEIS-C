/*
 * A356438  Numbers k such that A309892(k) = k/gpf(k), where gpf = A006530.
 * https://oeis.org/A356438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1048, u32 n <= 36902, u64 n <= 36902,
 *            u128 n <= 36902, big any n
 * Validated: standard, pass, 82 terms through n = 9922 (2026-11-03)
 * Helpers:   largest_prime_factor, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A356438: a(n) is the nth smallest integer k such that
   A309892(k) = k/gpf(k), where gpf is the largest prime factor.
   Equivalently for k ≥ 2: k/gpf(k) < nextprime(gpf(k)). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/largest_prime_factor.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* Keep a static list of all terms found so far  */
    static seq_t *terms = NULL;
    static size_t term_len = 0;
    static size_t term_cap = 0;
    static idx_t next_k = 2;                /* next integer to test */

    /* Add the trivial first term */
    if (term_len == 0) {
        seq_t one = 1;
        append_candidate(&terms, &term_len, &term_cap, one);
    }

    /* Generate terms until we have at least n of them */
    while (term_len < n) {
        /* treat k as 64‑bit integer for the helper functions */
        uint64_t k = (uint64_t)next_k;

        /* largest prime factor and its successor prime */
        uint64_t p  = largest_prime_factor(k);   /* p ≥ 2 for k ≥ 2 */
        uint64_t np = next_prime(p);

        /* test the inequality:  k < p * nextprime(p)  */
        seq_t prod = (seq_t)p * (seq_t)np;        /* product fits in seq_t */
        if ((seq_t)k < prod) {
            append_candidate(&terms, &term_len, &term_cap, (seq_t)k);
        }

        ++next_k;
    }

    return terms[n-1];
}
