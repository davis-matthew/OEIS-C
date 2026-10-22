/*
 * A307755  Exponential convolution of partition numbers (A000041) with
 *          themselves.
 * https://oeis.org/A307755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * p(k) * p(n-k) where p(k) is
 *            the partition number (A000041)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 44, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *p_cache = NULL;    /* partition numbers */
static idx_t  p_max   = 0;        /* highest index computed for p_cache */
static size_t p_cap   = 0;        /* allocated capacity for p_cache */

static seq_t *a_cache = NULL;    /* values of the sequence */
static idx_t  a_max   = 0;        /* highest index computed for a_cache */
static size_t a_cap   = 0;        /* allocated capacity for a_cache */

static seq_t *sigma_cache = NULL;/* sum of divisors σ(k) */
static idx_t  sigma_max   = 0;    /* highest index computed for sigma_cache */
static size_t sigma_cap   = 0;    /* allocated capacity for sigma_cache */

/* Ensure that an array allocated via malloc/realloc has capacity for
   indices 0 .. n (inclusive). */
static void
ensure_cap(seq_t **arr, size_t *cap, idx_t n)
{
    if (*cap <= (size_t)n) {
        size_t new_cap = (*cap ? (*cap << 1) : 1);
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* ------------------------------------------------------------------
       Compute σ(k) = sum of divisors for k = 1 .. n
       ------------------------------------------------------------------ */
    ensure_cap(&sigma_cache, &sigma_cap, n);
    for (idx_t i = 1; i <= n; ++i) sigma_cache[i] = 0;
    for (idx_t d = 1; d <= n; ++d) {
        seq_t dseq = (seq_t)d;
        for (idx_t m = d; m <= n; m += d) {
            sigma_cache[m] += dseq;
        }
    }

    /* ------------------------------------------------------------------
       Compute partition numbers p(0..n) using the recurrence
          p(0) = 1,
          p(i) = (1/i) * Σ_{k=1..i} σ(k) * p(i-k)
       ------------------------------------------------------------------ */
    ensure_cap(&p_cache, &p_cap, n);
    if (p_max == 0) {
        p_cache[0] = (seq_t)1;
        p_max = 0;
    }
    for (idx_t i = p_max + 1; i <= n; ++i) {
        seq_t total = 0;
        for (idx_t k = 1; k <= i; ++k) {
            total += sigma_cache[k] * p_cache[i - k];
        }
        p_cache[i] = total / (seq_t)i;
    }
    p_max = n;

    /* ------------------------------------------------------------------
       Compute a(n) for all values up to n using
          a(m) = Σ_{k=0..m} binomial(m,k) * p(k) * p(m-k)
       ------------------------------------------------------------------ */
    ensure_cap(&a_cache, &a_cap, n);
    if (a_max == 0) {
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }
    for (idx_t m = a_max + 1; m <= n; ++m) {
        seq_t sum   = 0;
        seq_t binom = (seq_t)1; /* binomial(m,0) */
        for (idx_t k = 0; k <= m; ++k) {
            seq_t term = binom;
            term *= p_cache[k];
            term *= p_cache[m - k];
            sum += term;
            if (k < m) {
                binom = binom * (seq_t)(m - k);
                binom = binom / (seq_t)(k + 1);
            }
        }
        a_cache[m] = sum;
    }
    a_max = n;

    return a_cache[n];
}
