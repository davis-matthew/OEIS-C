/*
 * A295834  Expansion of e.g.f. Product_{k>=1} (1 + x^k)^((-1)^(k+1)/k).
 * https://oeis.org/A295834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n≥1, a(n) = Σ_{k=1}^{n} s(k) * (n-1)!/(n-k)! *
 *            a(n-k), where s(k) = Σ_{d|k} (-1)^{d + k/d}
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 23,
 *            big any n
 * Helpers:   compute_s_up_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the e.g.f. product from the OEIS entry. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_s_up_to.h"

/* static caches for computed values */
static seq_t *a_cache = NULL;          /* a(n) values      */
static size_t a_cap   = 0;              /* size of a_cache   */
static idx_t  a_max_computed = 0;       /* highest index computed */

static seq_t *s_cache = NULL;          /* s(n) values      */
static size_t s_cap   = 0;              /* size of s_cache */

/* ensure s_cache holds values up to at least sz-1 elements */
static void ensure_s_size(size_t sz)
{
    if (s_cap >= sz) return;

    size_t new_cap = s_cap ? s_cap : 1;
    while (new_cap < sz) new_cap <<= 1;

    seq_t *new_s = (seq_t *)realloc(s_cache, new_cap * sizeof(seq_t));
    if (!new_s) abort(); /* allocation failure */

    compute_s_up_to((idx_t)(new_cap - 1), new_s);

    s_cache = new_s;
    s_cap   = new_cap;
}

/* ensure a_cache holds values up to at least sz-1 elements */
static void ensure_a_size(size_t sz)
{
    if (a_cap >= sz) return;

    size_t new_cap = a_cap ? a_cap : 1;
    while (new_cap < sz) new_cap <<= 1;

    seq_t *new_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_a) abort(); /* allocation failure */

    a_cache = new_a;
    a_cap   = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* allocate tables large enough for this index */
    ensure_a_size((size_t)n + 1);
    ensure_s_size((size_t)n + 1);

    /* initialize a(0) if necessary */
    if (a_cache[0] == 0) a_cache[0] = 1;

    if (n <= a_max_computed)
        return a_cache[n];

    for (idx_t i = a_max_computed + 1; i <= n; ++i) {
        if (i == 0) { a_cache[0] = 1; continue; }

        seq_t total   = 0;
        seq_t product = 1;                 /* (i-1)*(i-2)*...*(i-j) */

        for (idx_t j = 0; j <= i - 1; ++j) {
            idx_t k = j + 1;                     /* k runs 1..i */
            seq_t term = s_cache[k];
            term *= a_cache[i - 1 - j];
            term *= product;
            total += term;
            if (j < i - 1)
                product *= (seq_t)(idx_t)(i - 1 - j);
        }
        a_cache[i] = total;
    }

    a_max_computed = n;
    return a_cache[n];
}
