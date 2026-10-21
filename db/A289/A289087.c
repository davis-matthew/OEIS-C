/*
 * A289087  Imaginary parts of the recursive sequence a(n+2) = Sum_{k=0..n}
 *          binomial(n,k)*a(k)*a(n+1-k), with a(0)=1, a(1)=2i.
 * https://oeis.org/A289087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = imag(2*L0*tan(L0*z + L1)) where L0 = sqrt(i-1/4) and L1 =
 *            arccos(sqrt(1+i/4)). The recurrence is: a(0) = 1, a(1) = 2i
 *            a(n+2) = Σ_{k=0..n} C(n,k) * a(k) * a(n+1-k) for n ≥ 0 This
 *            function returns the imaginary part A289087(n)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 14, i64 n <= 21, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declarations for seq_t arithmetic provided by the build. */
extern seq_t binomial(idx_t n, idx_t k);  /* Not used directly but kept for reference. */

/* Static cache for real and imaginary parts. */
static seq_t *real_cache = NULL;
static seq_t *imag_cache = NULL;
static size_t cache_cap  = 0;         /* Allocated size of the arrays. */
static idx_t  cache_last = 0;         /* Highest index that has been computed. */
static bool initialized = false;

/* Ensure that the cache can hold indices up to 'need' (inclusive). */
static void ensure_capacity(idx_t need)
{
    if ((size_t)need + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 2;
    while ((size_t)need + 1 > new_cap)
        new_cap <<= 1;
    seq_t *new_real = (seq_t *)realloc(real_cache, new_cap * sizeof(seq_t));
    seq_t *new_imag = (seq_t *)realloc(imag_cache, new_cap * sizeof(seq_t));
    if (!new_real || !new_imag) {
        /* Allocation failed; abort to avoid undefined behaviour. */
        abort();
    }
    real_cache = new_real;
    imag_cache = new_imag;
    cache_cap = new_cap;
    /* New elements are uninitialised; they will be set before first use. */
}

/* Initialise the cache with the two base values. */
static void init_cache(void)
{
    if (initialized)
        return;
    ensure_capacity(1);            /* Need indices 0 and 1. */
    /* Base values: a(0) = 1 + 0i. */
    real_cache[0] = (seq_t)1;
    imag_cache[0] = (seq_t)0;
    /* a(1) = 0 + 2i. */
    real_cache[1] = (seq_t)0;
    imag_cache[1] = (seq_t)2;
    cache_last = 1;
    initialized = true;
}

/* The required external function. */
seq_t a(idx_t n)
{
    init_cache();

    if (n < cache_last)            /* Cached value available. */
        return imag_cache[n];

    /* Grow cache if necessary. */
    ensure_capacity(n);

    /* Compute missing terms up to n. */
    for (idx_t m = cache_last + 1; m <= n; ++m) {
        /* Recurrence: a(m) = Σ_{k=0}^{m-2} C(m-2,k) * a(k) * a(m-1-k). */
        idx_t r = m - 2;          /* r = n in the recurrence formula. */
        seq_t sumR = (seq_t)0;
        seq_t sumI = (seq_t)0;
        seq_t binom = (seq_t)1;   /* binom(r,0) */

        for (idx_t k = 0; k <= r; ++k) {
            idx_t j = r + 1 - k;   /* j = m-1-k */

            /* Retrieve a(k) = rk + i*ik, a(j) = rj + i*ij. */
            seq_t rk = real_cache[k];
            seq_t ik = imag_cache[k];
            seq_t rj = real_cache[j];
            seq_t ij = imag_cache[j];

            /* (rk * rj - ik * ij) + i*(rk * ij + ik * rj) */
            seq_t termR = rk * rj - ik * ij;
            seq_t termI = rk * ij + ik * rj;

            sumR += binom * termR;
            sumI += binom * termI;

            if (k < r) {
                /* binom(r,k+1) = binom * (r-k) / (k+1) */
                binom = (binom * (seq_t)(r - k)) / (seq_t)(k + 1);
            }
        }

        real_cache[m] = sumR;
        imag_cache[m] = sumI;
    }

    cache_last = n;
    return imag_cache[n];
}
