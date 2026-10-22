/*
 * A367831  E.g.f. A(x) satisfies A(x) = (1 + (exp(x) - 1) * A(3*x)) / (1 -
 *          x).
 * https://oeis.org/A367831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = n*a(n-1) + Σ_{k=1..n} 3^(n-k) * binomial(n,k) *
 *            a(n-k) (OEIS A367831)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

/* ===== Dynamic array helpers (valid for both C and C++) ===== */

/* Ensure *arr has capacity at least 'needed' elements.
   Elements are default-constructed (C++) or left uninitialized (C).
   Existing elements are copied to the new storage.                 */
static void ensure_capacity_seq(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;

    /* Allocate new buffer */
    void *mem = malloc(needed * sizeof(seq_t));
    if (!mem) abort(); /* allocation failure */

#ifdef __cplusplus
    seq_t *newarr = static_cast<seq_t *>(mem);
#else
    seq_t *newarr = (seq_t *)mem;
#endif

    /* Copy existing elements */
    for (size_t i = 0; i < *cap; ++i)
        newarr[i] = (*arr)[i];

#ifdef __cplusplus
    /* Default-construct new elements */
    for (size_t i = *cap; i < needed; ++i)
        new (newarr + i) seq_t();

    /* Destroy old elements */
    for (size_t i = 0; i < *cap; ++i)
        (*arr)[i].~seq_t();
#endif

    /* Free old storage and update pointers */
    free(*arr);
    *arr   = newarr;
    *cap   = needed;
}

/* ===== Sequence computation ===== */

static seq_t *cache   = NULL;
static size_t cache_cap   = 0;
static idx_t  last_cached = 0;

static seq_t *pow3   = NULL;
static size_t pow3_cap   = 0;
static idx_t  last_pow3 = 0;

seq_t a(idx_t n)
{
    /* First‑time initialization */
    if (cache == NULL)
    {
        ensure_capacity_seq(&cache,   &cache_cap,   1); /* cache[0] */
        cache[0] = (seq_t)1;

        ensure_capacity_seq(&pow3,   &pow3_cap,   1); /* pow3[0] */
        pow3[0] = (seq_t)1;

        last_cached = 0;
        last_pow3   = 0;
    }

    if (n <= last_cached)
        return cache[n];

    /* Grow arrays to accommodate index n */
    size_t target = (size_t)n + 1;          /* number of elements needed */
    ensure_capacity_seq(&cache,   &cache_cap,   target);
    ensure_capacity_seq(&pow3,   &pow3_cap,   target);

    /* Compute missing terms */
    for (idx_t i = last_cached + 1; i <= n; ++i)
    {
        /* compute 3^i */
        if (i > 0)
            pow3[i] = pow3[i-1] * (seq_t)3;

        /* n * a(n-1) term */
        seq_t sum = (seq_t)i * cache[i-1];

        /* Summation part */
        seq_t binom = 1;   /* C(i,0) = 1 */
        for (idx_t k = 1; k <= i; ++k)
        {
            /* binom = C(i,k) = C(i,k-1) * (i-k+1) / k */
            binom = (binom * ((seq_t)(i - k + 1))) / (seq_t)k;

            seq_t term = pow3[i - k];
            term *= binom;
            term *= cache[i - k];
            sum += term;
        }
        cache[i] = sum;
    }

    last_cached = n;
    return cache[n];
}
