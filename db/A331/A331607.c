/*
 * A331607  E.g.f.: exp(1 / (1 - sin(x)) - 1).
 * https://oeis.org/A331607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n-1,k-1) * A000111(k+1) * a(n-k).
 *            Initial values: a(0)=1, a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000111(k) are Euler up/down numbers: 1,1,1,2,5,16,61,272,1385,7936,... */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binomial.h"

/*=== Helper to grow a dynamic array =====================*/
static void grow_cache(seq_t **ptr, size_t *cap, size_t needed)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap << 1;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *ptr = tmp;
    *cap = new_cap;
}

/*=== Euler up/down (A000111) numbers cache ----------------*/
static seq_t *e_cache = NULL;
static size_t   e_cap  = 0;
static size_t   e_len  = 0;   /* number of already computed values */

/* Ensure Euler numbers up to index `upto` are computed */
static void
ensure_euler(idx_t upto)
{
    if ((idx_t)e_len > upto) return;          /* already available */

    grow_cache(&e_cache, &e_cap, (size_t)upto + 1);

    if (e_len == 0) {
        e_cache[0] = (seq_t)1;                /* E(0) */
        e_len = 1;
    }
    if (e_len == 1 && upto >= 1) {
        e_cache[1] = (seq_t)1;                /* E(1) */
        e_len = 2;
    }

    /* Recurrence:  E(n) = ½ * Σ_{k=0}^{n-1} binomial(n-1,k) * E(k) * E(n-1-k) */
    for (idx_t m = e_len; m <= upto; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < m; ++k) {
            seq_t comb = binomial(m - 1, k);
            seq_t term = comb * e_cache[k];
            term *= e_cache[m - 1 - k];
            sum += term;
        }
        e_cache[m] = sum / 2;                  /* integer division (sum is always even) */
    }
    e_len = (size_t)upto + 1;
}

/*=== Sequence a(n) cache -----------------------------------*/
static seq_t *a_cache = NULL;
static size_t   a_cap  = 0;
static size_t   a_len  = 0;   /* number of computed a(n) */

/* Ensure a(0)..a(upto) are computed */
static void
ensure_a(idx_t upto)
{
    if ((idx_t)a_len > upto) return;          /* already available */

    grow_cache(&a_cache, &a_cap, (size_t)upto + 1);

    if (a_len == 0) {
        a_cache[0] = (seq_t)1;                /* a(0) */
        a_len = 1;
    }

    /* For the recurrence we need Euler numbers up to index (upto+1) */
    ensure_euler(upto + 1);

    /* DP: a(n) = Σ_{k=1..n} binomial(n-1,k-1) * A000111(k+1) * a(n-k) */
    for (idx_t cur = a_len; cur <= upto; ++cur) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t comb = binomial(cur - 1, k - 1);
            seq_t term = comb * e_cache[k + 1];   /* A000111(k+1) = E(k+1) */
            term *= a_cache[cur - k];
            sum += term;
        }
        a_cache[cur] = sum;
    }
    a_len = (size_t)upto + 1;
}

/*=== Public function -------------------------------------------*/
seq_t a(idx_t n)
{
    ensure_a(n);
    return a_cache[n];
}
