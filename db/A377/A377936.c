/*
 * A377936  Number of matchings in the complete planted binary tree with 2^n
 *          leaves.
 * https://oeis.org/A377936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = u(n) + v(n) * u(0) = v(0) = 1 * u(n) = v(n-1)^2 * v(n) =
 *            v(n-1)^2 + 2·v(n-1)·u(n-1) * a(0) = 2, a(1) = 4, …
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache for u and v values */
static seq_t *u_cache = NULL;
static seq_t *v_cache = NULL;
static idx_t cache_len = (idx_t)-1;   /* (idx_t)-1 means not yet initialized */

/* cross‑language allocation for an array of seq_t */
static seq_t *seq_alloc(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

/* reallocate and copy a seq_t array */
static seq_t *seq_realloc(seq_t *old, size_t old_len, size_t new_len)
{
#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_len];
    for (size_t i = 0; i < old_len; ++i)
        new_arr[i] = old[i];
    delete[] old;
    return new_arr;
#else
    return (seq_t *)realloc(old, new_len * sizeof(seq_t));
#endif
}

/* grow the cache up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* if already initialized and n <= cache_len, nothing to do */
    if (cache_len != (idx_t)-1 && n <= cache_len)
        return;

    /* initialise on first call or when cache_len == (idx_t)-1 */
    if (cache_len == (idx_t)-1) {
        idx_t init_len = 1;                     /* store index 0 */
        u_cache = seq_alloc(init_len);
        v_cache = seq_alloc(init_len);
        if (!u_cache || !v_cache) {
            /* allocation failed – keep uninitialised */
            return;
        }
        u_cache[0] = (seq_t)1;
        v_cache[0] = (seq_t)1;
        cache_len = 0;
    }

    /* enlarge arrays if necessary */
    if (n > cache_len) {
        size_t new_len = (size_t)(n + 1);
        size_t old_len = (size_t)(cache_len + 1);
        seq_t *u_new = seq_realloc(u_cache, old_len, new_len);
        seq_t *v_new = seq_realloc(v_cache, old_len, new_len);
        if (!u_new || !v_new) {
            /* fail: keep old state, leave cache_len unchanged */
            return;
        }
        u_cache = u_new;
        v_cache = v_new;

        /* compute new entries */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t v_prev = v_cache[i - 1];
            seq_t u_prev = u_cache[i - 1];

            seq_t vv = v_prev * v_prev;                    /* v(n‑1)^2 */
            u_cache[i] = vv;                               /* u(n) = vv */
            seq_t two_vu = v_prev * (seq_t)2 * u_prev;     /* 2*v(n‑1)*u(n‑1) */
            v_cache[i] = vv + two_vu;                      /* v(n) = vv + two_vu */
        }
        cache_len = n;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (!u_cache || !v_cache || n > cache_len)
        return (seq_t)0;   /* allocation failure or uninitialised */
    return u_cache[n] + v_cache[n];
}
