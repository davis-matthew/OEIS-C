/*
 * A372012  G.f. A(x) satisfies A(x) = 1/( 1 + x*(1 - 4*x*A(x))^(1/2) ).
 * https://oeis.org/A372012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * Σ_{k=0..n} 4^(n-k) * C(n,k) * C(k/2, n-k) /
 *            (n-k+1)
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 20, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation below uses the recurrences derived from
   the functional equation
        f(x) = 1/(1+x*sqrt(1-4x*f(x)))
   where f(x)=Σ a(n) x^n and s(x)=sqrt(1-4x*f(x))=Σ b(n) x^n.  The
   recurrences are
        b(0)=1,  b(n) = (-4*a(n-1) - Σ_{k=1}^{n-1} b(k)*b(n-k)) / 2,
        a(0)=1,  a(n) = - Σ_{k=0}^{n-1} a(k)*b(n-1-k).
   They give an O(n^2) algorithm that is fast enough for n≈10 000
   and works with arbitrary‑precision seq_t.                           */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static size_t cache_cap = 0;     /* allocated size of the arrays   */
static idx_t  max_computed = 0;  /* highest n for which a[n] is known */

/* Ensure the internal arrays can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    a_cache = seq_realloc(a_cache, cache_cap, new_cap);
    b_cache = seq_realloc(b_cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Return the n-th term of A372012.  The sequence is memoised. */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* initialise base case if this is the first call            */
    if (max_computed == 0) {
        a_cache[0] = (seq_t)1;
        b_cache[0] = (seq_t)1;
    }

    if (n <= max_computed) return a_cache[n];

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* --- compute b[i] --- */
        seq_t sum_b = 0;
        for (idx_t k = 1; k < i; ++k)
            sum_b += b_cache[k] * b_cache[i - k];

        /* (-4 * a[i-1] - sum_b) is always even, division gives integer */
        b_cache[i] = (- (seq_t)4 * a_cache[i - 1] - sum_b) / 2;

        /* --- compute a[i] --- */
        seq_t sum_a = 0;
        for (idx_t k = 0; k < i; ++k)
            sum_a += a_cache[k] * b_cache[i - 1 - k];

        a_cache[i] = -sum_a;
    }

    max_computed = n;
    return a_cache[n];
}
