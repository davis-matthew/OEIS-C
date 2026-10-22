/*
 * A328730  Dirichlet g.f.: Product_{k>=2} (1 - k^(-s))^k.
 * https://oeis.org/A328730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = - Sum_{d|n, d<n} A050367(n/d) * a(d) where A050367 is the
 *            weighted number of unordered factorizations of n with factors
 *            >1; it is computed by depth‑first enumeration
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"   /* seq_alloc_new_array() */
#include "../utils/seq_realloc.h"          /* seq_realloc() */

/* ---------- Helper: binomial coefficient (n choose k) for small k ---------- */
static seq_t binom_unsigned(idx_t n, unsigned k)
{
    seq_t res = 1;
    for (unsigned i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* ---------- Depth‑first search of unordered factorizations ---------- */
static void dfs_factor(idx_t rem, idx_t min_factor, int depth,
                       unsigned *factors, seq_t *total)
{
    if (rem == 1)                 /* finished a factorization */
    {
        seq_t weight = 1;
        int i = 0;
        while (i < depth)
        {
            unsigned k = factors[i];
            int cnt = 1;
            ++i;
            while (i < depth && factors[i] == k)
            {
                ++cnt;
                ++i;
            }
            weight = weight * binom_unsigned(k + cnt - 1, (unsigned)cnt);
        }
        *total = *total + weight;
        return;
    }

    for (idx_t d = min_factor; d * d <= rem; ++d)
    {
        if (rem % d == 0)
        {
            factors[depth] = (unsigned)d;
            dfs_factor(rem / d, d, depth + 1, factors, total);
        }
    }

    /* last factor = rem itself */
    factors[depth] = (unsigned)rem;
    dfs_factor(1, rem, depth + 1, factors, total);
}

/* ---------- Compute A050367(n) ---------- */
static void compute_A050367(idx_t n, seq_t *out)
{
    if (n == 1)
    {
        *out = 1;
        return;
    }

    seq_t total = 0;
    unsigned factors[64];     /* depth never exceeds ~14 for n ≤ 10000 */
    dfs_factor(n, 2, 0, factors, &total);
    *out = total;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Static cache for f(n)=A050367(n) and for a(n) itself. */
    static seq_t *f = NULL;          /* cache for A050367 values */
    static size_t f_cap = 0;         /* current capacity of f[]   */
    static seq_t *a_arr = NULL;      /* cache for a(n) values     */
    static size_t a_cap = 0;         /* current capacity of a_arr */
    static idx_t cached = 0;         /* largest index computed so far */

    if (n > cached)
    {
        /* Grow capacity if necessary (allocate one extra for 1‑based indexing). */
        size_t newcap = (size_t)n + 1;
        if (f_cap < newcap)
        {
            f = seq_realloc(f, f_cap, newcap);
            f_cap = newcap;
            if (f_cap > 0)
                f[0] = 0;            /* unused, but give a defined value */
        }
        if (a_cap < newcap)
        {
            a_arr = seq_realloc(a_arr, a_cap, newcap);
            a_cap = newcap;
            if (a_cap > 0)
                a_arr[0] = 0;
        }

        /* Compute all missing entries in ascending order. */
        for (idx_t m = cached + 1; m <= n; ++m)
        {
            if (m == 1)                 /* initial conditions */
            {
                f[1]     = 1;
                a_arr[1] = 1;
                continue;
            }

            /* Step 1: compute A050367(m) */
            compute_A050367(m, &f[m]);

            /* Step 2: compute a(m) via the recurrence */
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= m; ++d)
            {
                if (m % d == 0)
                {
                    idx_t d1 = d;
                    idx_t d2 = m / d;

                    if (d1 < m)
                        sum = sum + f[d2] * a_arr[d1];
                    if (d2 != d1 && d2 < m)
                        sum = sum + f[d1] * a_arr[d2];
                }
            }
            a_arr[m] = -sum;
        }

        cached = n;
    }

    return a_arr[n];
}
