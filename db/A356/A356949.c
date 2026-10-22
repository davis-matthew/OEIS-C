/*
 * A356949  E.g.f. satisfies log(A(x)) = x^2 * (exp(x) - 1) * A(x).
 * https://oeis.org/A356949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A356949
 *  Formula: a(n) = n! * Σ_{k=0..⌊n/3⌋} (k+1)^{k-1} * Stirling2(n-2k,k)/(n-2k)!
 *  In this implementation we use the following recurrences derived from the
 *  exponential generating function:
 *      b_n = Σ_{k=3}^{n} binom(n,k) * k*(k-1) * a_{n-k}
 *      a_n = Σ_{k=1}^{n} binom(n-1,k-1) * b_k * a_{n-k}
 *   where b_n is the coefficient of log(A(x)) and a_0 = 1.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for sequence terms and intermediary b_n values. */
static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static idx_t cache_size = 0;

/* Ensure the cache can store terms up to index n (inclusive).
 * Uses new/delete in C++ and malloc/realloc in C. */
static void ensure_cache(idx_t n)
{
    if (n+1 <= cache_size) return;

    idx_t new_size = n + 1;

#if defined(__cplusplus)
    seq_t *a_new = new seq_t[new_size];
    seq_t *b_new = new seq_t[new_size];

    if (a_cache != NULL) {
        /* copy old values */
        for (idx_t i = 0; i < cache_size; ++i) {
            a_new[i] = a_cache[i];
            b_new[i] = b_cache[i];
        }
        delete[] a_cache;
        delete[] b_cache;
    }
    a_cache = a_new;
    b_cache = b_new;
#else
    a_cache = realloc(a_cache, new_size * sizeof(seq_t));
    b_cache = realloc(b_cache, new_size * sizeof(seq_t));
#endif

    /* initialize new entries to zero */
    for (idx_t i = cache_size; i < new_size; ++i) {
        a_cache[i] = 0;
        b_cache[i] = 0;
    }
    /* set a[0] = 1 on first allocation */
    if (cache_size == 0) a_cache[0] = 1;

    cache_size = new_size;
}

/* Compute all sequence values up to index n using the recurrences. */
static void compute_up_to(idx_t n)
{
    idx_t old_size = cache_size;
    if (n+1 <= cache_size) return;

    ensure_cache(n);
    idx_t start = (old_size == 0) ? 1 : old_size;

    for (idx_t i = start; i <= n; ++i) {

        /* ---- compute b[i] ---- */
        seq_t bi = 0;
        if (i >= 3) {
            /* initial binomial C(i,3) = i*(i-1)*(i-2)/6 */
            seq_t binom = ((seq_t)i * (i - 1) * (i - 2)) / 6;
            for (idx_t k = 3; k <= i; ++k) {
                seq_t term = binom * ((seq_t)k * (k - 1)) * a_cache[i - k];
                bi += term;
                if (k < i) {
                    binom = binom * (i - k) / (k + 1); /* C(i,k+1) */
                }
            }
        }
        b_cache[i] = bi;

        /* ---- compute a[i] ---- */
        seq_t ai = 0;
        seq_t binom2 = 1;            /* C(i-1,0) */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = binom2 * b_cache[k] * a_cache[i - k];
            ai += term;
            if (k < i) {
                binom2 = binom2 * (i - k) / k;  /* C(i-1,k) */
            }
        }
        a_cache[i] = ai;
    }
}

/* Public interface required by the testing harness. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_cache[n];
}
