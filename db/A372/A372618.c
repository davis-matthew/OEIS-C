/*
 * A372618  a(1) = a(2) = 1; a(n+2) = Sum_{k=1..n} a(gcd(n,k)).
 * https://oeis.org/A372618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1; a(n) = sum_{d|n-2} φ(d) * a((n-2)/d) (n≥3)
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Simple integer square‑root for idx_t (≤ 64‑bit)                     */
/* ------------------------------------------------------------------ */
static inline idx_t int_isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) <= n / (r + 1))
        ++r;
    return r;
}

/* ------------------------------------------------------------------ */
/*  Reallocate a seq_t array keeping existing elements.               */
/* ------------------------------------------------------------------ */
static seq_t *realloc_seq(seq_t *old, size_t old_cap, size_t new_cap)
{
    /* new_cap is the maximum valid index we want to support   */
    seq_t *newp = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!newp) exit(1);
    if (old)
    {
        for (size_t i = 0; i <= old_cap; ++i)
            newp[i] = old[i];
        free(old);
    }
    return newp;
}

/* ------------------------------------------------------------------ */
/*  φ (Euler totient) table, recomputed on demand                     */
/* ------------------------------------------------------------------ */
static seq_t *phi_cache = NULL;
static size_t phi_cap   = 0;   /* maximum index computed (inclusive) */

static void recompute_phi(size_t new_cap)
{
    seq_t *new_phi = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_phi) exit(1);

    for (size_t i = 0; i <= new_cap; ++i)
        new_phi[i] = (seq_t)i;

    for (size_t p = 2; p <= new_cap; ++p)
    {
        if (new_phi[p] == (seq_t)p)          /* prime */
        {
            for (size_t j = p; j <= new_cap; j += p)
                new_phi[j] = new_phi[j] - new_phi[j] / (seq_t)p;
        }
    }

    free(phi_cache);
    phi_cache = new_phi;
    phi_cap   = new_cap;
}

static void ensure_phi(size_t m)
{
    if (m > phi_cap)
        recompute_phi(m);
}

/* ------------------------------------------------------------------ */
/*  Cache for a(n)                                                      */
/* ------------------------------------------------------------------ */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0;   /* maximum index currently cached (inclusive) */

static void compute_up_to(size_t up_to)
{
    if (up_to <= a_cap)
        return;

    a_cache = realloc_seq(a_cache, a_cap, up_to);
    for (size_t i = a_cap + 1; i <= up_to; ++i)
        a_cache[i] = 0;          /* sentinel for uncomputed entries */

    for (size_t i = 1; i <= up_to; ++i)
    {
        if (a_cache[i])
            continue;            /* already computed */
        if (i <= 2)
        {
            a_cache[i] = 1;
            continue;
        }

        idx_t m = (idx_t)i - 2;        /* n-2 in the recurrence  */
        ensure_phi((size_t)m);

        seq_t sum = 0;
        idx_t sqrt_m = int_isqrt(m);
        for (size_t d = 1; d <= (size_t)sqrt_m; ++d)
        {
            if (m % d == 0)
            {
                size_t d2 = (size_t)(m / d);
                sum += phi_cache[d2] * a_cache[d];
                if (d2 != d)
                    sum += phi_cache[d] * a_cache[d2];
            }
        }
        a_cache[i] = sum;
    }
    a_cap = up_to;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n<1 */
    compute_up_to((size_t)n);
    return a_cache[(size_t)n];
}
