/*
 * A251577  E.g.f.: exp(7*x*G(x)^6) / G(x)^6 where G(x) = 1 + x*G(x)^7 is the
 *          g.f. of A002296.
 * https://oeis.org/A251577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A251577 – e.g.f.  A(x) = exp( Σ_{n≥1} A130565(n)*x^n/n )
 *  where A130565(n) = binomial(7*n-2,n)/(6*n-1).
 *
 *  Recurrence derived from A'(x)=A(x)*f'(x) with
 *      f'(x)=Σ_{k≥1} A130565(k)*x^{k-1}:
 *
 *      a(0) = 1
 *      for n ≥ 1:
 *          a(n) = Σ_{k=1}^{n} A130565(k) * a(n-k) *
 *                 ( (n-1)! / (n-k)! )
 *  The factor (n-1)!/(n-k)! = product_{t=n-k+1}^{n-1} t,
 *  which is updated incrementally in the inner loop.
 *
 *  Memoised arrays are used for both a(n) and A130565(k).
 *  The routine works for any order of calls and in all supported
 *  integer precisions (uint8_t … __int128, arbitrary‑precision).
 *
 *  External types:
 *   - idx_t  : unsigned 64‑bit integer
 *   - seq_t  : unsigned integer type chosen by the build
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

typedef uint64_t idx_t;   /* supplied by the build */

/* ---------- Memoised arrays ---------- */
static seq_t *a_cache = NULL;   /* a_cache[n] = a(n)   */
static size_t a_cap  = 0;
static idx_t  a_max  = (idx_t)-1;   /* max n already computed */

static seq_t *f_cache = NULL;   /* f_cache[k] = A130565(k) */
static size_t f_cap  = 0;
static idx_t  f_max  = 0;      /* max k already computed (0 unused) */

/* Ensure alloc array has at least `needed` slots; expand by powers of two. */
static void
ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : *cap;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Compute A130565(k) for all k ≤ n and store in f_cache. */
static void
ensure_f(idx_t n)
{
    if (n <= f_max) return;          /* already up to date */
    ensure_capacity(&f_cache, &f_cap, n + 1);
    for (idx_t k = f_max + 1; k <= n; ++k) {
        seq_t bin = binom(7 * k - 2, k);          /* binomial(7k-2,k) */
        seq_t denom = (seq_t)(6 * k - 1);          /* 6k-1 */
        f_cache[k] = bin / denom;                 /* exact division */
    }
    f_max = n;
}

/* Compute a(n) with memoisation. */
seq_t
a(idx_t n)
{
    /* Ensure enough space for a_cache. */
    size_t needed = (size_t)(n + 1);
    ensure_capacity(&a_cache, &a_cap, needed);

    /* Initialise a(0) once. */
    if (a_max == (idx_t)-1) {
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }

    /* Compute any missing a(k) up to n. */
    if (n > a_max) {
        for (idx_t i = a_max + 1; i <= n; ++i) {
            /* Ensure A130565 values needed for this i. */
            ensure_f(i);

            seq_t sum = 0;
            seq_t r   = 1;   /* r = (i-1)!/(i-k)! for current k */

            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = f_cache[k];            /* A130565(k) */
                term *= a_cache[i - k];             /* a(i-k)   */
                term *= r;                          /* factor   */
                sum  += term;                       /* accumulate */

                /* update r for next k (skip after last k) */
                if (k < i) {
                    r *= (seq_t)(i - k);
                }
            }
            a_cache[i] = sum;
        }
        a_max = n;
    }

    return a_cache[n];
}
