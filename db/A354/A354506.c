/*
 * A354506  a(n) = n! * Sum_{k=1..n} ( Sum_{d|k} (-1)^(k/d+1) )/(k * (n-k)!).
 * https://oeis.org/A354506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1..n} A048272(k) * (k-1)! * C(n,k) where A048272(k)
 *            = Σ_{d|k} (-1)^(k/d+1) and C(n,k) is the binomial coefficient.
 *            This implementation precomputes factorials, A048272(k), and A(n)
 *            values up to the maximum requested n, using memoisation. All
 *            arithmetic is performed with the build‑suppl...
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required standard headers */
#include <stdlib.h>   /* malloc, realloc, abort */

/* Global caches (all indexed from 0). */
static idx_t cache_n = 0;              /* highest n cached so far      */
static seq_t *fact = NULL;             /* factorial[0..cache_n]         */
static int   *A   = NULL;              /* A048272(k) for k=0..cache_n   */
static seq_t *a_cache = NULL;          /* a(k) for k=0..cache_n         */

/* Ensure factorials, A array and a_cache are computed up to n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_n) return;

    /* Allocate new size */
    size_t new_size = (size_t)(n + 1);

    /* Expand factorial array */
    seq_t *new_fact = (seq_t *)realloc(fact, new_size * sizeof(seq_t));
    if (!new_fact) abort();
    fact = new_fact;

    /* Resize A array */
    int *new_A = (int *)realloc(A, new_size * sizeof(int));
    if (!new_A) abort();
    A = new_A;

    /* Resize a_cache array */
    seq_t *new_a = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    if (!new_a) abort();
    a_cache = new_a;

    /* Initialise factorial[0] if first time */
    if (cache_n == 0 && fact[0] == 0)
        fact[0] = (seq_t)1;

    /* Compute factorials and A values for new indices */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;

        /* Compute A048272(i) by enumerating divisors */
        int sum = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                idx_t d1 = d;
                idx_t d2 = i / d;

                /* divisor d1 */
                if (((i / d1 + 1) & 1) == 0)
                    sum += 1;
                else
                    sum -= 1;

                /* divisor d2 (if distinct) */
                if (d2 != d1) {
                    if (((i / d2 + 1) & 1) == 0)
                        sum += 1;
                    else
                        sum -= 1;
                }
            }
        }
        A[i] = sum;
    }

    /* Compute a(k) for new indices */
    for (idx_t m = cache_n + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)m;          /* C(m,1) */

        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = fact[k - 1] * (seq_t)A[k] * bin;
            sum += term;
            if (k < m) {
                bin = bin * (seq_t)(m - k);
                bin = bin / (seq_t)(k + 1);
            }
        }
        a_cache[m] = sum;
    }

    cache_n = n;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* values below offset are irrelevant */
    ensure_cache(n);
    return a_cache[n];
}
