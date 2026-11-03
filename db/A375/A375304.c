/*
 * A375304  Triangle read by rows, T(n,k) = Sum_{i=0..n}
 *          (-1)^i*(k!)^i*(n*k-i*k)!, 0<=k<=n.
 * https://oeis.org/A375304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Sum_{i=0..n} (-1)^i * (k!)^i * (n*k - i*k)!
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 13, u64 n <= 19, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows, a(0) is the first term. */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/mem.h"

static void ensure_fact(idx_t need)
{
    /* static cache of factorials */
    static seq_t *fac = NULL;
    static idx_t fac_cap = 0;
    static idx_t fac_len = 0;  /* last computed index */

    /* allocate new capacity if needed */
    if (need > fac_cap) {
        size_t new_cap = fac_cap > 0 ? fac_cap << 1 : 128;
        while ((idx_t)new_cap < need) new_cap <<= 1;

        seq_t *new_fac = mem_resize(seq_t, fac, (new_cap + 1));
        if (!new_fac) {
            /* out of memory; abort */
            abort();
        }
        fac = new_fac;
        fac_cap = (idx_t)new_cap;
    }

    /* compute missing factorials */
    if (fac_len == 0) {
        /* initialize fac[0] */
        fac[0] = (seq_t)1;
        fac_len = 0;
    }
    for (idx_t i = fac_len + 1; i <= need; ++i) {
        fac[i] = fac[i - 1] * (seq_t)i;
    }
    fac_len = need;
}

static seq_t factorial_at(idx_t idx)
{
    /* ensures factorial cache up to idx */
    static seq_t *fac = NULL;
    static idx_t fac_cap = 0;
    static idx_t fac_len = 0;

    if (fac == NULL) {
        fac = mem_new(seq_t, 1);
        if (!fac) abort();
        fac[0] = (seq_t)1;
        fac_cap = 0;
        fac_len = 0;
    }

    if (idx > fac_len) {
        /* expand cache */
        seq_t *new_fac = mem_resize(seq_t, fac, (idx + 1));
        if (!new_fac) abort();
        fac = new_fac;
        for (idx_t i = fac_len + 1; i <= idx; ++i) {
            fac[i] = fac[i - 1] * (seq_t)i;
        }
        fac_len = idx;
    }
    return fac[idx];
}

seq_t a(idx_t n)
{
    /* Find row r and column k such that n lies in row r */
    double d = sqrt((long double)8.0 * (long double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    /* adjust in case of rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    idx_t prefix = r * (r + 1) / 2;
    idx_t k = n - prefix;          /* 0 <= k <= r */

    /* Special case k == 0: T(r,0) = alternating sum of ones */
    if (k == 0) {
        return (r % 2 == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* Ensure factorials up to needed size */
    idx_t max_fact = k * r;
    ensure_fact(max_fact);
    idx_t max_k_fact = k;
    ensure_fact(max_k_fact);

    seq_t fact_k = factorial_at(k);

    seq_t sum = 0;
    seq_t pow_factor = 1;  /* (k!)^t, starts with t=0 */

    for (idx_t t = 0; t <= r; ++t) {
        /* factorial of k*(r - t) */
        idx_t idx_fact = k * (r - t);
        seq_t term = pow_factor * factorial_at(idx_fact);

        if (t & 1)
            sum -= term;
        else
            sum += term;

        if (t < r)
            pow_factor *= fact_k;  /* advance to next power */
    }
    return sum;
}
