/*
 * A337741  Numbers all of whose divisors are Niven numbers (A005349).
 * https://oeis.org/A337741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A337741: Numbers all of whose divisors are Niven numbers.
   The sequence is finite (31 terms, the last being 1944).  For n larger than the
   last term, the function returns 0.  The implementation uses memoisation
   to support arbitrary call order and a small search that stops after 31 terms. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"   /* provides seq_t digit_sum(seq_t) */

/* Test if the (positive) integer v is a Niven number. */
static bool is_niven(idx_t v)
{
    seq_t x = (seq_t)v;
    seq_t s = digit_sum(x);
    /* s > 0 for all positive v */
    return x % s == (seq_t)0;
}

/* Test whether all divisors of n are Niven numbers. */
static bool all_divisors_niven(idx_t n)
{
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            if (!is_niven(d)) return false;
            idx_t e = n / d;
            if (e != d && !is_niven(e))
                return false;
        }
    }
    return true;
}

/* Main sequence function.  The array `terms` holds the sequence values up to the
   largest index requested so far.  The search continues only while
   terms_len < 31; after that, no further terms exist. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined below offset */
    static seq_t *terms = NULL;            /* cached terms, 1‑based indexing */
    static size_t terms_len = 0;           /* number of cached terms         */
    static size_t terms_cap = 0;           /* current capacity of `terms`    */

    /* Allocate initial cache if needed. */
    if (terms == NULL) {
        terms_cap = 64;
        terms = (seq_t *)malloc(sizeof(seq_t) * terms_cap);
        if (!terms) return (seq_t)0;       /* allocation failure */
    }

    /* Build cache up to the requested index or until we have 31 terms. */
    while (terms_len < n && terms_len < 31) {

        /* Start searching from the next integer after the last found term. */
        idx_t candidate = (terms_len > 0) ?
                          (idx_t)terms[terms_len-1] + (idx_t)1 : (idx_t)1;

        /* Search until we find the next valid term or we exceed a safe bound. */
        bool found = false;
        for (;;) {

            /* Avoid an infinite loop in the unlikely case that no further
               terms exist beyond the known 1944.  The bound 2^31 is far
               above the largest term. */
            if (candidate > (idx_t)2000000000) break;

            if (all_divisors_niven(candidate)) {
                /* Ensure enough capacity. */
                if (terms_len >= terms_cap) {
                    terms_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(terms, sizeof(seq_t) * terms_cap);
                    if (!tmp) { free(terms); terms = NULL; return (seq_t)0; }
                    terms = tmp;
                }
                terms[terms_len++] = (seq_t)candidate;
                found = true;
                break;
            }
            ++candidate;
        }
        /* If we didn't find a new term, the sequence is exhausted. */
        if (!found) break;
    }

    /* Return the nth term if it was computed, otherwise 0. */
    if (n <= terms_len)
        return terms[n-1];   /* sequence is 1‑based */
    else
        return (seq_t)0;     /* no such term */
}
