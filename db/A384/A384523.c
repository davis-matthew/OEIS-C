/*
 * A384523  Expansion of e.g.f. 3/(7 - 4*exp(3*x)).
 * https://oeis.org/A384523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * Sum_{k=1..n} 3^(k‑1) * binomial(n,k) * a(n‑k), with
 *            a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static arrays for memoisation */
static seq_t *a_cache  = NULL;
static size_t  a_cap   = 0;
static idx_t   a_max   = (idx_t)-1;      /* largest index computed */

static seq_t *pow3_cache = NULL;
static size_t  pow3_cap   = 0;

/* Ensure a_cache has room for indices 0..need */
static void ensure_a_cap(idx_t need)
{
    if (a_cap <= (size_t)need) {
        size_t new_cap = a_cap ? a_cap << 1 : 64;
        while (new_cap <= (size_t)need) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        a_cache = new_cache;
        a_cap   = new_cap;
    }
}

/* Ensure pow3_cache has room for indices 0..need and is initialised */
static void ensure_pow3_cap(idx_t need)
{
    if (pow3_cap <= (size_t)need) {
        size_t old_cap = pow3_cap;
        size_t new_cap = old_cap ? old_cap << 1 : 64;
        while (new_cap <= (size_t)need) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(pow3_cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        pow3_cache = new_cache;

        if (old_cap == 0) {               /* first time: build from scratch */
            pow3_cache[0] = (seq_t)1;
            if (new_cap > 1) pow3_cache[1] = (seq_t)3;
            for (size_t i = 2; i < new_cap; ++i)
                pow3_cache[i] = pow3_cache[i-1] * (seq_t)3;
        } else {                          /* extend existing table */
            for (size_t i = old_cap; i < new_cap; ++i)
                pow3_cache[i] = pow3_cache[i-1] * (seq_t)3;
        }
        pow3_cap = new_cap;
    }
}

/* Compute all a[i] up to n (inclusive) */
static void compute_a_up_to(idx_t n)
{
    if (a_cache == NULL) {               /* initialise cache */
        a_cap = 1;
        a_cache = (seq_t*)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }
    if (n <= a_max) return;              /* already computed */

    ensure_a_cap(n);

    for (idx_t i = a_max + 1; i <= n; ++i) {
        ensure_pow3_cap(i - 1);          /* need 3^(i‑1) */

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)i;          /* C(i,1) */
        seq_t pow3_k_minus1 = pow3_cache[0]; /* 3^0 */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = a_cache[i - k];
            term = term * pow3_k_minus1 * binom;
            sum += term;

            if (k < i) {
                pow3_k_minus1 = pow3_k_minus1 * (seq_t)3;
                seq_t top = (seq_t)(i - k);
                binom = binom * top;
                binom = binom / (seq_t)(k + 1);
            }
        }
        sum = sum * (seq_t)4;
        a_cache[i] = sum;
    }
    a_max = n;
}

/* Public entry point requested by the framework */
seq_t a(idx_t n)
{
    compute_a_up_to(n);
    return a_cache[n];
}
