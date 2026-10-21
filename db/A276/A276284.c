/*
 * A276284  a(0) = a(1) = a(2) = a(3) = a(4) = 1; for n > 4, a(n) = (
 *          a(n-1)+a(n-3)+1 )*( a(n-2)+a(n-4)+1 ) / a(n-5).
 * https://oeis.org/A276284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)+a(n-3)+1)*(a(n-2)+a(n-4)+1)/a(n-5) with
 *            a(0)=a(1)=a(2)=a(3)=a(4)=1; n>=5
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑n (definition + initial values).  */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* The sequence type and index type are supplied externally */
extern seq_t a(idx_t n);

/*--- Internal cache ---*/
#if defined(__cplusplus)
static std::vector<seq_t> cache_vec;
#else
static seq_t *cache_arr = NULL;
static size_t  cache_arr_size = 0;
#endif
static idx_t max_computed = 0;    /* maximum index for which cache_vec/arr holds values */

/* Ensure cache has space up to 'n' (inclusive). */
static void ensure_capacity(idx_t n) {
    idx_t needed = n + 1;
#if defined(__cplusplus)
    if (cache_vec.size() < (size_t)needed) {
        cache_vec.resize(needed);
    }
#else
    if (cache_arr_size < (size_t)needed) {
        size_t new_size = (size_t)needed;
        seq_t *new_arr;
        if (!cache_arr) {
            new_arr = (seq_t*)malloc(new_size * sizeof(seq_t));
        } else {
            new_arr = (seq_t*)realloc(cache_arr, new_size * sizeof(seq_t));
        }
        if (!new_arr) {
            /* Out of memory – terminate (definition assumes sufficient memory) */
            abort();
        }
        cache_arr = new_arr;
        cache_arr_size = new_size;
    }
#endif
}

/* Compute a(n).  The function may be called in any order; results are cached. */
seq_t a(idx_t n) {
    /* Base values for indices 0..4 */
    if (n <= 4) return (seq_t)1;

    ensure_capacity(n);

    /* Initialize base values if not yet cached */
    if (max_computed < 4) {
#if defined(__cplusplus)
        for (idx_t i = 0; i <= 4; ++i) cache_vec[i] = (seq_t)1;
#else
        for (idx_t i = 0; i <= 4; ++i) cache_arr[i] = (seq_t)1;
#endif
        max_computed = 4;
    }

    /* Compute missing values up to n */
    if (max_computed < n) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val;
#if defined(__cplusplus)
            val = ((cache_vec[i-1] + cache_vec[i-3] + (seq_t)1) *
                   (cache_vec[i-2] + cache_vec[i-4] + (seq_t)1)) /
                  cache_vec[i-5];
            cache_vec[i] = val;
#else
            val = ((cache_arr[i-1] + cache_arr[i-3] + (seq_t)1) *
                   (cache_arr[i-2] + cache_arr[i-4] + (seq_t)1)) /
                  cache_arr[i-5];
            cache_arr[i] = val;
#endif
        }
        max_computed = n;
    }

#if defined(__cplusplus)
    return cache_vec[n];
#else
    return cache_arr[n];
#endif
}
