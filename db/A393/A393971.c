/*
 * A393971  Number of partitions p of n into distinct parts such that max(p)
 *          <= 3*min(p).
 * https://oeis.org/A393971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=1..n} [q^{n-j}] ∏_{k=j+1..3j} (1+q^k)
 * Range:     u8 n <= 55, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   poly_div_term, poly_mul_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/poly_div_term.h"
#include "../utils/poly_mul_term.h"

/* Memoisation storage */
static idx_t max_cached = 0;
static seq_t *cached_ans = NULL;

/* compute all terms up to Nmax (inclusive) */
static void compute_to(idx_t Nmax)
{
    size_t len = (size_t)(Nmax + 1);
    seq_t *a   = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *poly= (seq_t *)calloc(len, sizeof(seq_t));
    if (!a || !poly) abort();

    /* initial polynomial for j = 1 : product_{k=2..3} (1+q^k) */
    poly[0] = (seq_t)1;
    poly_mul_term(poly, 2, len);
    poly_mul_term(poly, 3, len);

    for (idx_t j = 1; j <= Nmax; ++j) {
        size_t jidx = (size_t)j;

        /* add contributions for this j */
        for (size_t nidx = jidx; nidx < len; ++nidx) {
            a[nidx] += poly[nidx - jidx];
        }
        if (j == Nmax) break;

        /* transition to polynomial for j+1 */
        poly_div_term(poly, j + 1, len);          /* remove factor (1+q^{j+1}) */
        poly_mul_term(poly, 3 * j + 1, len);      /* add new factors */
        poly_mul_term(poly, 3 * j + 2, len);
        poly_mul_term(poly, 3 * j + 3, len);
    }

    /* store the computed values in the cache */
    seq_t *new_ans = (seq_t *)malloc(len * sizeof(seq_t));
    if (!new_ans) abort();
    for (size_t i = 0; i < len; ++i) new_ans[i] = a[i];

    free(cached_ans);
    cached_ans = new_ans;
    max_cached = Nmax;

    free(a);
    free(poly);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence is undefined for n=0 */
    if (n > max_cached) compute_to(n);
    return cached_ans[n];
}
