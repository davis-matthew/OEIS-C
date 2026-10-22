/*
 * A336068  Numbers k such that the exponent of the highest power of 3
 *          dividing k (A007949) is a divisor of k.
 * https://oeis.org/A336068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336068: the n‑th positive integer k for which v3(k) (the
   exponent of the highest power of 3 dividing k) divides k.
   Computation: brute‑force search with memoisation; indices start at
   1 (a(1)=3). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *terms      = NULL;   /* cached terms          */
static idx_t  terms_len  = 0;      /* number of terms       */
static idx_t  terms_cap  = 0;      /* allocated capacity    */
static idx_t  next_k     = 1;      /* candidate to test     */

/* Ensure that the terms array can hold at least 'needed' elements. */
static void ensure_terms_cap(idx_t needed)
{
    if (terms_cap < needed) {
        idx_t new_cap = terms_cap == 0 ? 4 : terms_cap * 2;
        if (new_cap < needed)
            new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof *terms);
        if (!tmp) /* allocation failure – give up */
            abort();
        terms      = tmp;
        terms_cap  = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (terms_len < n) {
        seq_t k = (seq_t)next_k;
        /* Compute v3(k), the exponent of 3 in k. */
        seq_t t = k;
        unsigned int e = 0;
        while (t % (seq_t)3 == 0) {
            t /= (seq_t)3;
            ++e;
        }

        /* Store k if e > 0 and e divides k. */
        if (e > 0) {
            if ((k % (seq_t)e) == 0) {
                ensure_terms_cap(terms_len + 1);
                terms[terms_len++] = k;
            }
        }
        ++next_k;
    }
    return terms[n - 1]; /* offset 1 */
}
