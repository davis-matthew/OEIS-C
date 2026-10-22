/*
 * A302866  a(n) = 2*(n*(2*n+1)*b(n-1) + (-1)^(n-1)*(2*n+1)*((n-1)!)^2), with
 *          a(0) = 0, n > 1.
 * https://oeis.org/A302866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*( n*(2n+1)*b(n-1) + (-1)^(n-1)*(2n+1)*((n-1)!)^2 ), *
 *            with a(0)=0. The recurrence is implemented directly
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for previously computed terms */
static seq_t *a_cache = NULL;   /* array of a(n) values                */
static size_t a_cap   = 0;      /* current capacity of a_cache         */
static idx_t  a_max   = 0;      /* highest n computed in a_cache       */
static seq_t  fact_lr = (seq_t)1; /* running factorial: (a_max)! */

static void ensure_a(idx_t needed)
{
    if (needed <= a_max)
        return;          /* nothing to do */

    /* grow the cache array to include 'needed' */
    size_t new_cap = a_cap ? a_cap * 2 : 1;
    while (new_cap <= (size_t)needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    a_cache = tmp;
    a_cap   = new_cap;

    /* compute new values from a_max+1 up to needed */
    for (idx_t i = a_max + 1; i <= needed; ++i) {
        /* t1 = a(i-1) * i * (2*i+1) */
        seq_t t1 = a_cache[i - 1];
        t1 *= (seq_t)i;
        t1 *= (seq_t)(2 * i + 1);

        /* factorial of (i-1) is fact_lr           */
        seq_t fact_sq = fact_lr * fact_lr;            /* ((i-1)!)^2   */
        seq_t sum     = (seq_t)(2 * i + 1) * fact_sq; /* (2i+1)*fact_sq */

        /* sign: (-1)^(i-1) => + for odd i, - for even i */
        if (i & 1)
            t1 += sum;   /* odd i -> (+1) */
        else
            t1 -= sum;   /* even i -> (-1) */

        a_cache[i] = t1 * (seq_t)2;   /* final a(i) */

        /* update factorial for next iteration: fact_lr *= i */
        fact_lr *= (seq_t)i;
    }
    a_max = needed;
}

seq_t a(idx_t n)
{
    /* lazy initialization of the cache */
    if (!a_cache) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache)
            abort();
        a_cache[0] = (seq_t)0;  /* a(0) = 0 */
        a_cap   = 1;
        a_max   = 0;
        fact_lr = (seq_t)1;     /* 0! = 1 */
    }

    ensure_a(n);
    return a_cache[n];
}
