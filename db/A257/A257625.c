/*
 * A257625  Triangle read by rows: T(n, k) = t(n-k, k), where t(0,0) = 1,
 *          t(n,m) = 0 if n < 0 or m < 0, else t(n,m) = f(m)*t(n-1,m) +
 *          f(n)*t(n,m-1), and f(n) = 6*n + 3.
 * https://oeis.org/A257625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A257625 – triangle read by rows. T(n,k) = t(n-k,k) where
 *            t(0,0) = 1, t(n,m) = 0 if n<0 or m<0, otherwise t(n,m) =
 *            f(m)*t(n-1,m)+f(n)*t(n,m-1), and f(n) = 6*n + 3. The function
 *            returns the n-th term of the flattened triangle
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 38, u64 n <= 96, u128 n <= 284,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Allocation helpers – work in C and C++ */
#ifdef __cplusplus
static inline seq_t *alloc_array(size_t len)
{
    return new seq_t[len]();          /* value‑initialized to zero */
}
static inline void free_array(seq_t *p)
{
    delete[] p;
}
#else
static inline seq_t *alloc_array(size_t len)
{
    return (seq_t*)calloc(len, sizeof(seq_t));
}
static inline void free_array(seq_t *p)
{
    free(p);
}
#endif

seq_t a(idx_t n)
{
    /* static state – built only once and remembered across calls */
    static seq_t *prev = NULL;          /* previous row */
    static size_t prev_cap = 0;
    static seq_t *cur  = NULL;          /* current row (temporary) */
    static size_t cur_cap = 0;
    static seq_t *cache = NULL;         /* flattened triangle cache */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;        /* number of values cached */
    static idx_t  last_row = 0;         /* index of last computed row */

    /* first run – initialise */
    if (prev == NULL)
    {
        prev_cap = 1;
        prev = alloc_array(prev_cap);
        prev[0] = (seq_t)1;                       /* t(0,0) = 1 */

        cur_cap = 1;
        cur  = alloc_array(cur_cap);               /* will be reused */

        cache_cap = 1;
        cache = alloc_array(cache_cap);
        cache[0] = (seq_t)1;                       /* a(0) */
        cache_len = 1;
        last_row = 0;
    }

    /* build rows until the cache contains the requested index n */
    while (cache_len <= n)
    {
        idx_t    next_row = last_row + 1;
        size_t   len      = (size_t)next_row + 1;

        /* ensure current row buffer is large enough */
        if (cur_cap < len)
        {
            free_array(cur);
            cur = alloc_array(len);
            cur_cap = len;
        }

        /* row 0 is already handled; compute from row 1 onward */
        cur[0] = (seq_t)3 * prev[0];              /* f(0)=3 */

        /* inner elements (for k = 1 .. next_row-1) */
        for (idx_t k = 1; k < next_row; ++k)
        {
            seq_t f_k     = (seq_t)6 * (seq_t)k + (seq_t)3;
            seq_t f_row_k = (seq_t)6 * (seq_t)(next_row - k) + (seq_t)3;
            cur[k] = f_k * prev[k] + f_row_k * prev[k-1];
        }

        /* last element is symmetric to the first */
        cur[next_row] = cur[0];

        /* grow cache if necessary */
        size_t need_cache = cache_len + len;
        if (cache_cap < need_cache)
        {
            seq_t *new_cache = alloc_array(need_cache);
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free_array(cache);
            cache = new_cache;
            cache_cap = need_cache;
        }

        /* copy newly computed row into the cache */
        for (size_t i = 0; i < len; ++i)
            cache[cache_len + i] = cur[i];
        cache_len += len;

        /* copy current row into prev for the next iteration */
        if (prev_cap < len)
        {
            seq_t *new_prev = alloc_array(len);
            for (size_t i = 0; i < len; ++i)
                new_prev[i] = cur[i];
            free_array(prev);
            prev = new_prev;
            prev_cap = len;
        }
        else
        {
            for (size_t i = 0; i < len; ++i)
                prev[i] = cur[i];
        }

        last_row = next_row;
    }

    return cache[n];
}
