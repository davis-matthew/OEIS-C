/*
 * A308024  G.f. A(x) satisfies: A(x) = x * (1 + (1/(1 + x)) * (A(x/(1 + x)) +
 *          A(x^2/(1 + x)^2) + A(x^3/(1 + x)^3) + ...)).
 * https://oeis.org/A308024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 12, i32 n <= 19, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308024
   Formula: a(1) = 1;
   a(n+1) = Σ_{k=1..n} (-1)^{n-k} * C(n,k) * Σ_{d|k} a(d)
   (indices start at 1) */

#include <stdlib.h>
#include <stdint.h>

/* static storage for memoisation */
static seq_t *a_cache   = NULL;      /* a[1..]   */
static seq_t *div_sum  = NULL;      /* Σ_{d|k} a(d)  */
static size_t cache_cap = 0;        /* allocated size (indices 0..cache_cap-1) */
static idx_t  last_computed = 0;    /* greatest n that has a[n] filled */

/* helper: recompute divisor sums from scratch for the current cache */
static void recompute_divisor_sums(size_t upto)
{
    /* Zero the array */
    for (size_t i = 0; i < upto; ++i)
        div_sum[i] = 0;

    /* Add contributions of all computed a[d] */
    for (idx_t d = 1; d <= last_computed; ++d)
    {
        seq_t val = a_cache[d];
        for (idx_t m = d; m < (idx_t)upto; m += d)
            div_sum[m] += val;
    }
}

/* helper: ensure the cache holds indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n)
        return;

    size_t new_cap = (size_t)n + 1;            /* indices 0..n */

    a_cache  = (seq_t *)realloc(a_cache,  new_cap * sizeof(seq_t));
    div_sum  = (seq_t *)realloc(div_sum,  new_cap * sizeof(seq_t));

    /* Initialise the new elements to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        div_sum[i] = 0;

    /* If this is the first call, set a[1] = 1 and initialise the sums */
    if (last_computed == 0)
    {
        a_cache[1] = 1;
        last_computed = 1;
    }

    /* Recompute all divisor sums for the enlarged cache */
    recompute_divisor_sums(new_cap);
    cache_cap = new_cap;
}

/* main function required by the test suite */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* below offset is undefined */

    ensure_capacity(n);

    /* Compute missing terms up to n */
    while (last_computed < n)
    {
        idx_t next = last_computed + 1;         /* index of a[next] */
        idx_t m    = next - 1;                  /* n in the recurrence */

        seq_t total = 0;
        seq_t binom = 1;                        /* C(m,0) */

        for (idx_t k = 1; k <= m; ++k)
        {
            /* binom = binom * (m - k + 1) / k   */
            binom = binom * (seq_t)(m - k + 1) / (seq_t)k;

            seq_t term = binom * div_sum[k];

            /* apply the sign (-1)^{m-k} */
            if ( ((m - k) & 1) )
                term = -term;

            total += term;
        }

        a_cache[next] = total;

        /* Update divisor sums with the new term */
        for (idx_t m = next; m < (idx_t)cache_cap; m += next)
            div_sum[m] += a_cache[next];

        last_computed = next;
    }

    return a_cache[n];
}
