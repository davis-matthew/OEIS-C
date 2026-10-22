/*
 * A335986  E.g.f. A(x) satisfies: A'(x) = 1 + A(1 - exp(x)).
 * https://oeis.org/A335986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 11, i32 n <= 17, i64 n <= 24, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A335986
 *
 *  a(1) = 1
 *  a(n) = ∑_{k=1}^{n-1} (-1)^k * S(n-1,k) * a(k)   for n ≥ 2
 *          where S(m,k) are the Stirling numbers of the second kind.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */

/* --------------------------------------------------------------------- */
/*  Global cache for the sequence values and the current Stirling‑2 row */
/* --------------------------------------------------------------------- */
static seq_t *a_cache = NULL;          /* a(k) for k = 0 .. computed_n   */
static seq_t *s_cache = NULL;          /* S(k) for current row           */
static size_t cache_cap = 0;           /* allocated size (>= computed_n+1) */
static idx_t  computed_n = 0;          /* last n for which a(n) is known   */

/* --------------------------------------------------------------------- */
/*  Ensure the cache is large enough to contain indices up to "need"      */
/* --------------------------------------------------------------------- */
static void ensure_cache(idx_t need)
{
    size_t required = (size_t)need + 1;
    if (required > cache_cap)
    {
        size_t new_cap = required * 2;
        seq_t *new_a = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        seq_t *new_s = (seq_t *)realloc(s_cache, new_cap * sizeof(seq_t));
        if (!new_a || !new_s) { abort(); }      /* allocation failure */

        /* initialise new slots to zero */
        for (size_t i = cache_cap; i < new_cap; ++i)
        {
            new_a[i] = (seq_t)0;
            new_s[i] = (seq_t)0;
        }

        a_cache = new_a;
        s_cache = new_s;
        cache_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
/*  Initialise the cache for the first time                                 */
/* --------------------------------------------------------------------- */
static void init_sequence(void)
{
    ensure_cache(1);            /* at least index 1 */
    a_cache[1] = (seq_t)1;      /* a(1) */
    s_cache[1] = (seq_t)1;      /* S(1,1)=1   */
    s_cache[0] = (seq_t)0;      /* S(.,0)=0   */
    computed_n = 1;
}

/* --------------------------------------------------------------------- */
/*  Public function ----------------------------------------------------- */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* defined only for n≥1 */

    if (!a_cache) init_sequence();          /* first call */

    ensure_cache(n);                        /* make room for index n */

    /* compute any missing terms up to n */
    while (computed_n < n)
    {
        idx_t new_n = computed_n + 1;       /* next term to compute */

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < new_n; ++k)
        {
            seq_t prod = s_cache[k] * a_cache[k];
            sum += ((k & 1) ? -prod : prod);  /* (-1)^k */
        }
        a_cache[new_n] = sum;

        /* advance Stirling‑2 row: S(new_n,k) = k*S(new_n-1,k) + S(new_n-1,k-1) */
        for (idx_t k = new_n; k >= 1; --k)
        {
            s_cache[k] = (seq_t)k * s_cache[k] + s_cache[k-1];
        }
        s_cache[0] = (seq_t)0;                /* S(.,0)=0 */

        computed_n = new_n;
    }

    return a_cache[n];
}
