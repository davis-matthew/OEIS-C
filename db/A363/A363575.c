/*
 * A363575  G.f. satisfies A(x) = exp( Sum_{k>=1} A(x^k) * x^k/(k * (1 +
 *          x^k)^3) ).
 * https://oeis.org/A363575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 37, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A363575
 *
 * Recurrence derived from the functional equation
 *   A(x) = exp( Σ_{k≥1} A(x^k) * x^k / (k * (1 + x^k)^3) )
 *
 * Let a(n) be the coefficients: A(x) = Σ_{n≥0} a(n) x^n,  a(0)=1.
 *
 * Let
 *   T(t,d) = Σ_{j=0}^{t-1} a(t-1-j) * binom(2+j,j) * (-1)^j
 *   with binom(2+j,j) = (j+2)(j+1)/2.
 *
 * Define S(k) = k * s(k) = Σ_{d|k} (k/d) * T(k/d,d)
 *  (k*s(k) is integer because the factor k cancels the 1/d in s(k)).
 *
 * Then the coefficients satisfy
 *   a(0) = 1
 *   a(n) = (1/n) * Σ_{k=1}^{n} S(k) * a(n-k)   for n ≥ 1
 *
 * The implementation memoises the sequence so that calls with arbitrary
 * indices in any order are efficient.
 */

#include <stdlib.h>

static seq_t *a_cache   = NULL;  /* a[0..N]   */
static seq_t *S_cache   = NULL;  /* S[1..N]   */
static size_t cache_cap = 0;     /* current capacity   */
static idx_t   last_computed = 0;/* largest n already computed */

/* Helper: ensures the static arrays have capacity ≥ n+1 */
static void ensure_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap? cache_cap*2 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        a_cache = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
        S_cache = (seq_t*)realloc(S_cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Main function required by the testing harness */
seq_t a(idx_t n)
{
    /* a(0) is the first term of the sequence */
    if (n == (idx_t)0) return (seq_t)1;

    /* If the cache is empty initialise a[0] and S[0] */
    if (a_cache == NULL) {
        ensure_capacity(n);
        a_cache[0] = (seq_t)1;
        S_cache[0] = (seq_t)0;
        last_computed = 0;
    }

    /* Compute all terms up to n if necessary */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* ----------------------------------------------------------------
         * 1. Compute S(i) = i * s(i)
         *    S(i) = Σ_{d|i} (i/d) * T(i/d, d)
         * ---------------------------------------------------------------- */
        seq_t S_i = 0;
        /* Iterate over all divisors d of i */
        idx_t d;
        for (d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                /* first divisor d */
                idx_t t = i / d;          /* t = i/d */
                seq_t T = 0;
                idx_t j;
                for (j = 0; j < t; ++j) {
                    idx_t m = t - 1 - j;
                    seq_t binom = ((seq_t)(j + 2) * (j + 1)) / 2;
                    seq_t term  = a_cache[m] * binom;
                    if (j & 1) term = -term;   /* (-1)^j */
                    T += term;
                }
                S_i += (seq_t)t * T;

                /* second divisor i/d if distinct and not equal to d */
                d = i / d;  /* use temporary d2 = i/d variable */
                if (d != (i / d)) { /* always true because d already divided */
                    idx_t t2 = i / d; /* t2 = i/d2 = d */
                    seq_t T2 = 0;
                    for (j = 0; j < t2; ++j) {
                        idx_t m = t2 - 1 - j;
                        seq_t binom = ((seq_t)(j + 2) * (j + 1)) / 2;
                        seq_t term  = a_cache[m] * binom;
                        if (j & 1) term = -term;
                        T2 += term;
                    }
                    S_i += (seq_t)t2 * T2;
                }

                /* reset d for the next iteration of the outer loop */
                d = i / d; /* this restores original d for the loop */
            }
        }
        /* If i is a perfect square we need to include the divisor only once */
        if ((idx_t)(d * d) == i) {
            /* d == sqrt(i), already processed once above; do nothing */
        }
        S_cache[i] = S_i;

        /* ----------------------------------------------------------------
         * 2. Compute a(i) using the recurrence
         *    a(i) = (1/i) * Σ_{k=1}^{i} S(k) * a(i-k)
         * ---------------------------------------------------------------- */
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += S_cache[k] * a_cache[i - k];
        }
        a_cache[i] = sum / (seq_t)i;
    }

    last_computed = n;
    return a_cache[n];
}
