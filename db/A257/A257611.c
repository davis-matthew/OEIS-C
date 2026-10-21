/*
 * A257611  Triangle, read by rows, T(n,k) = t(n-k, k) where t(n,m) =
 *          f(m)*t(n-1,m) + f(n)*t(n,m-1) and f(n) = 2*n + 3.
 * https://oeis.org/A257611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r = ⌊(√(8n+1)-1)/2⌋ and k = n - r(r+1)/2.
 *            T(n,k) = t(n-k,k). The triangle t is defined by t(0,0) = 1,
 *            t(n,m) = 0 if n<0 or m<0, t(n,m) = f(m) * t(n-1,m) + f(n) *
 *            t(n,m-1) (n,m>0) with f(x) = 2x + 3. Initial values: a(0)=1,
 *            a(1)=3, a(2)=3, a(3)=9, a(4)=30,
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 57, u64 n <= 141,
 *            u128 n <= 415, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = (idx_t)1ULL << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) {
            x -= r + bit;
            r = (r >> 1) + bit;
        } else {
            r >>= 1;
        }
        bit >>= 2;
    }
    return r;
}

/* Cached table t(i,j) for 0 ≤ i,j ≤ cache_size.  */
static seq_t *t_cache = NULL;
static idx_t cache_size = 0;

/* Allocate or extend the cache to cover at least (new_size) rows. */
static void ensure_cache(idx_t new_size)
{
    /* The cache already covers the required size, unless it is still
       empty (cache_size==0).  */
    if (new_size <= cache_size && cache_size != 0)
        return;

    size_t stride = (size_t)(new_size + 1);
    size_t new_len = stride * stride;
    seq_t *new_tbl = NULL;
#ifdef __cplusplus
    new_tbl = new seq_t[new_len];
#else
    new_tbl = (seq_t *)malloc(new_len * sizeof(seq_t));
#endif
    if (!new_tbl) return;                    /* allocation failed */

    /* Build t(i,j) from 0..new_size. */
    for (idx_t i = 0; i <= new_size; ++i) {
        seq_t f_i = (seq_t)((idx_t)2 * i + 3);
        for (idx_t j = 0; j <= new_size; ++j) {
            size_t pos = (size_t)i * stride + j;
            if (i == 0 && j == 0) {
                new_tbl[pos] = (seq_t)1;
                continue;
            }
            seq_t f_j = (seq_t)((idx_t)2 * j + 3);
            seq_t val = 0;
            if (i > 0)
                val += f_j * new_tbl[(size_t)(i - 1) * stride + j];
            if (j > 0)
                val += f_i * new_tbl[(size_t)i * stride + (j - 1)];
            new_tbl[pos] = val;
        }
    }

    /* Replace old cache. */
    if (t_cache) {
#ifdef __cplusplus
        delete[] t_cache;
#else
        free(t_cache);
#endif
    }
    t_cache    = new_tbl;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    /* Determine which row (r) and column (k) in the triangle the index
       n falls into.  */
    idx_t r = (isqrt_u64((idx_t)8 * n + 1) - 1) >> 1;
    if (r == 0)                         /* special case: a(0) */
        return (seq_t)1;

    /* Ensure that the cached table contains at least rows up to r. */
    ensure_cache(r);

    idx_t start = r * (r + 1) / 2;
    idx_t k     = n - start;
    idx_t i     = r - k;   /* i = n-k for T(n,k)=t(n-k,k) */
    idx_t j     = k;

    size_t stride = (size_t)(cache_size + 1);
    size_t idx    = (size_t)i * stride + j;
    return t_cache[idx];
}
