/*
 * A246000  Number of length 5+2 0..n arrays with no pair in any consecutive
 *          three terms totalling exactly n.
 * https://oeis.org/A246000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 24, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A246000
   Formula:
     a(n) = 3*a(n-1) + 2*a(n-2) - 14*a(n-3) + 5*a(n-4) + 25*a(n-5)
            - 20*a(n-6) - 20*a(n-7) + 25*a(n-8) + 5*a(n-9)
            - 14*a(n-10) + 2*a(n-11) + 3*a(n-12) - a(n-13)
   Initial values:
     a(1)=2,   a(2)=38,   a(3)=956,   a(4)=7132,
     a(5)=40590, a(6)=153906, a(7)=496088, a(8)=1319480,
     a(9)=3178490, a(10)=6857470, a(11)=13855572,
     a(12)=26075028, a(13)=46848326
   The function below implements this recurrence with memoisation.
*/

#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *cache = NULL;   /* memoised terms */
static idx_t  cache_len = 0;  /* number of terms stored */
static idx_t  cache_cap = 0;  /* capacity of cache array */

/* 13 initial terms of the sequence */
static const seq_t init_terms[13] = {
    (seq_t)2,          /* a(1) */
    (seq_t)38,         /* a(2) */
    (seq_t)956,        /* a(3) */
    (seq_t)7132,       /* a(4) */
    (seq_t)40590,      /* a(5) */
    (seq_t)153906,     /* a(6) */
    (seq_t)496088,     /* a(7) */
    (seq_t)1319480,    /* a(8) */
    (seq_t)3178490,    /* a(9) */
    (seq_t)6857470,    /* a(10) */
    (seq_t)13855572,   /* a(11) */
    (seq_t)26075028,   /* a(12) */
    (seq_t)46848326    /* a(13) */
};

/* increase the cache capacity to at least 'new_cap' */
static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    /* double capacity until sufficient */
    size_t cap = cache_cap ? cache_cap * 2 : 16;
    while (cap < new_cap)
        cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = (idx_t)cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)         /* offset is 1, n<1 unused */
        return (seq_t)0;

    /* trivial case: n within the paid‑in initial terms */
    if (n <= 13)
        return init_terms[n - 1];

    /* initialise cache with the first 13 terms once */
    if (cache_len == 0)
    {
        ensure_capacity(16);          /* start with a small capacity */
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = init_terms[i];
        cache_len = 13;
    }

    /* extend cache up to the requested term */
    while (cache_len < n)
    {
        idx_t idx = cache_len;   /* zero‑based index of the new term */

        seq_t pos = 0;
        seq_t neg = 0;

        /* positive contributions */
        pos += cache[idx - 1] * (seq_t)3;   /* a(n-1) */
        pos += cache[idx - 2] * (seq_t)2;   /* a(n-2) */
        pos += cache[idx - 4] * (seq_t)5;   /* a(n-4) */
        pos += cache[idx - 5] * (seq_t)25;  /* a(n-5) */
        pos += cache[idx - 8] * (seq_t)25;  /* a(n-8) */
        pos += cache[idx - 9] * (seq_t)5;   /* a(n-9) */
        pos += cache[idx - 11] * (seq_t)2;  /* a(n-11) */
        pos += cache[idx - 12] * (seq_t)3;  /* a(n-12) */

        /* negative contributions */
        neg += cache[idx - 3] * (seq_t)14;  /* a(n-3) */
        neg += cache[idx - 6] * (seq_t)20;  /* a(n-6) */
        neg += cache[idx - 7] * (seq_t)20;  /* a(n-7) */
        neg += cache[idx - 10] * (seq_t)14; /* a(n-10) */
        neg += cache[idx - 13] * (seq_t)1;  /* a(n-13) */

        seq_t next = pos - neg;             /* guaranteed non‑negative */

        /* grow the cache if necessary and store the new term */
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = next;
    }

    return cache[n - 1];
}
