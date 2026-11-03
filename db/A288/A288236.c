/*
 * A288236  Coefficients in the expansion of 1/([r]-[2*r]*x+[3*r]*x^2-...); [
 *          ]=floor, r=-4/5+sqrt(6).
 * https://oeis.org/A288236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 35 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A288236
 *  Coefficients in the expansion of 1/(∑_{k≥0} floor((k+1)*r)*(-x)^k),
 *  where r = -4/5 + sqrt(6).
 *  Sequence is obtained as the power‑series coefficients of the reciprocal.
 *  Recurrence:  a(0)=1,
 *               a(n)=∑_{k=1}^n floor((k+1)*r)*(-1)^{k+1}*a(n‑k)
 *               (for n≥1).  This is equivalent to a(n)=odd_sum – even_sum
 *               where odd_sum/even_sum are sums of terms with odd/even k.
 */

#include <math.h>
#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static const double r = -0.8 + sqrt(6.0);   /* r = -4/5 + sqrt(6) */

static seq_t *cache = NULL;      /* cached values a(0)..a(N) */
static size_t cache_capacity = 0;
static size_t cache_size = 0;

/* floor((k+1)*r) – k is a positive integer (idx_t) */
static inline int floor_k(idx_t k)
{
    /* k up to ~10^4, no overflow */
    double val = (double)(k + 1) * r;
    return (int)floor(val + 1e-12);
}

/* ensure that cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    if ((idx_t)cache_size > n)
        return;

    /* enlarge cache if needed */
    size_t new_cap = cache_capacity ? cache_capacity : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    if (new_cap != cache_capacity)
    {
        cache = (seq_t *)seq_realloc(cache, cache_capacity, new_cap);
        cache_capacity = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t odd_sum  = (seq_t)0;
        seq_t even_sum = (seq_t)0;

        for (idx_t k = 1; k <= i; ++k)
        {
            seq_t term = (seq_t)floor_k(k) * cache[i - k];
            if (k & 1)               /* k odd → + */
                odd_sum += term;
            else                     /* k even → – */
                even_sum += term;
        }
        cache[i] = odd_sum - even_sum;    /* odd_sum ≥ even_sum */
    }
    cache_size = (size_t)n + 1;
}

/* external function required by the framework */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
