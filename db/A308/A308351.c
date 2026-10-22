/*
 * A308351  For n >= 2, a(n) = n*u(n-1) + n*(n-1)*u(n-2), where u = A292932;
 *          a(1)=1.
 * https://oeis.org/A308351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*A292932(n-1) + n*(n-1)*A292932(n-2) (n>=2), a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
/* When compiled as C++, allocate with new[] and delete[] so that
   arbitrary‑precision types are properly constructed and destroyed. */
static seq_t *cache = nullptr;
static idx_t  cap    = 0;   /* number of allocated slots               */
static idx_t  last   = 0;   /* highest index already computed (>=0)    */

static void grow_cache(idx_t n)
{
    if (n + 1 <= cap) return;
    size_t newcap = n + 1;
    seq_t *newarr = new seq_t[newcap];               /* constructs elements */
    if (cache) {
        for (size_t i = 0; i < cap; ++i)
            newarr[i] = cache[i];
        delete[] cache;
    }
    cache = newarr;
    cap   = newcap;
}
#else
/* In C, seq_t is a plain integral type so malloc is sufficient. */
static seq_t *cache = NULL;
static idx_t  cap    = 0;
static idx_t  last   = 0;

static void grow_cache(idx_t n)
{
    if (n + 1 <= cap) return;
    idx_t newcap = n + 1;
    seq_t *newarr = (seq_t*)malloc(newcap * sizeof(seq_t));
    if (!newarr) exit(1);         /* allocation failure */
    if (cache) {
        for (idx_t i = 0; i < cap; ++i)
            newarr[i] = cache[i];
        free(cache);
    }
    cache = newarr;
    cap   = newcap;
}
#endif

static void init_cache(void)
{
    if (cache) return;                /* already initialized    */
    grow_cache(1);                    /* allocate at least 2 slots (0..1) */
    cache[0] = (seq_t)0;              /* a(0) not used in OEIS but convenient */
    cache[1] = (seq_t)1;              /* a(1) */
    last    = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* not part of OEIS offset */
    init_cache();
    grow_cache(n);

    /* Compute missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        /* S = Σ_{k=0}^{i-1} binom(i,k) * a(k)   */
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;        /* binom(i,0) */

        for (idx_t k = 0; k < i; ++k)
        {
            sum += cache[k] * binom;
            binom = (binom * (seq_t)(i - k)) / (seq_t)(k + 1);
        }

        /* rhs = 1 if i==1, 2 if i==2, else 0 */
        seq_t rhs = (i == 1) ? (seq_t)1
                    : (i == 2) ? (seq_t)2
                    : (seq_t)0;

        seq_t term = (sum * 2) - (cache[i - 1] * (seq_t)i) + rhs;
        cache[i] = term;
    }
    last = n;
    return cache[n];
}
