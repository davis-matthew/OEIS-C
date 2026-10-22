/*
 * A307918  Number of (undirected) paths in the n-dipyramidal graph.
 * https://oeis.org/A307918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n*(-135 + 293*n - 180*n^2 + 80*n^3 - 15*n^4 + 2*n^5)/15
 * Recurrence (order 7): a(n)=7*a(n‑1)-21*a(n‑2)+35*a(n‑3)-35*a(n‑4)+21*a(n‑5)-7*a(n‑6)+a(n‑7)
 * Initial values (1 ≤ n ≤ 7): 3, 26, 111, 396, 1235, 3414, 8463
 *
 * The function `a` memoises the sequence in a static array so that any
 * order of calls is O(1) after the cache has been expanded.
 */

#include <stdlib.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <cstddef>
#endif

/* --------------- internal helpers --------------------------------- */
static seq_t *cache = NULL;   /* a(1) stored at cache[0]   */
static idx_t cache_len = 0;   /* number of terms stored     */

/* compute a(k) from the closed form when k ≤ 7 */
static seq_t compute_closed(idx_t k) {
    int64_t n      = (int64_t)k;
    int64_t val    = -135 + 293 * n - 180 * n * n
                     + 80 * n * n * n
                     - 15 * n * n * n * n
                     + 2  * n * n * n * n * n;
    int64_t numer  = n * val;
    int64_t res    = numer / 15;           /* exact division        */
    return (seq_t)res;
}

/* compute a(k) from the recurrence using the values in arr */
static seq_t compute_recurrence(idx_t k, const seq_t *arr) {
    /* indices of a(k‑1)..a(k‑7) in a 0‑based array */
    idx_t i = k - 1;
    seq_t pos = arr[i - 1] * (seq_t)7
              + arr[i - 3] * (seq_t)35
              + arr[i - 5] * (seq_t)21
              + arr[i - 7];
    seq_t neg = arr[i - 2] * (seq_t)21
              + arr[i - 4] * (seq_t)35
              + arr[i - 6] * (seq_t)7;
    return pos - neg;
}

/* internal wrapper that chooses the right method */
static seq_t compute_term(idx_t k, const seq_t *arr) {
    if (k <= 7) return compute_closed(k);
    return compute_recurrence(k, arr);
}

/* --------------- public function --------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out of range but harmless */

    /* enlarge the cache if needed */
    if (n > cache_len) {
        idx_t new_len = n;
#if defined(__cplusplus)
        /* C++ allocator: new/delete are safe for arbitrary classes */
        seq_t *new_cache = new seq_t[new_len];
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
            delete [] cache;
        }
        cache = new_cache;
#else
        /* C allocator: malloc/realloc return void*; cast explicitly   */
        if (cache == NULL)
            cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        else
            cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
#endif
        /* compute the missing terms */
        for (idx_t k = cache_len + 1; k <= n; ++k)
            cache[cache_len++] = compute_term(k, cache);
    }
    return cache[n - 1];  /* a(1) at index 0 */
}
