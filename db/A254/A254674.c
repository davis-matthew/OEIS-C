/*
 * A254674  Indices of heptagonal numbers (A000566) which are also centered
 *          triangular numbers (A005448).
 * https://oeis.org/A254674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 62*a(n-2) - 62*a(n-3) - a(n-4) + a(n-5) initial
 *            values: a(1) = 1, a(2) = 10, a(3) = 34, a(4) = 601, a(5) = 2089
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;   /* 1‑based terms, stored at indices [0..size-1] */
static idx_t  size  = 0;      /* number of terms currently stored   */
static idx_t  cap   = 0;      /* allocated capacity                  */

/* enlarge cache to at least NEW_CAP elements */
static void ensure_cache(idx_t new_cap) {
    if (new_cap <= cap)
        return;

    /* double the current capacity, or grow to the requested size,
       whichever is larger; avoid overflow on 2*cap */
    size_t new_capacity = (size_t)cap * 2;
    if (new_capacity == 0) new_capacity = 1;
    if (new_capacity < (size_t)new_cap)
        new_capacity = (size_t)new_cap;

    /* realloc may return NULL only if new_capacity == 0; that case
       is handled by the caller when cap == 0. */
    seq_t *tmp = (seq_t*)realloc(cache, new_capacity * sizeof(seq_t));
    if (!tmp && new_capacity > 0) {
        /* memory exhaustion – terminate */
        abort();
    }
    cache = tmp;

#ifdef __cplusplus
    /* Construct any newly allocated elements for a non‑trivial seq_t */
    for (idx_t i = cap; i < (idx_t)new_capacity; ++i)
        new (&cache[i]) seq_t();
#endif

    cap = (idx_t)new_capacity;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for non‑positive indices */

    if (n > size) {
        /* Allocate enough space to hold terms up to n */
        ensure_cache(n);

        /* Compute new terms from size+1 up to n */
        for (idx_t i = size; i < n; ++i) {
            idx_t idx = i + 1;   /* 1‑based index */

            if (idx == 1)
                cache[i] = 1;
            else if (idx == 2)
                cache[i] = 10;
            else if (idx == 3)
                cache[i] = 34;
            else if (idx == 4)
                cache[i] = 601;
            else if (idx == 5)
                cache[i] = 2089;
            else {
                seq_t term = cache[i-1];
                term += 62ULL * cache[i-2];
                term -= 62ULL * cache[i-3];
                term -= cache[i-4];
                term += cache[i-5];
                cache[i] = term;
            }
        }
        size = n;
    }
    return cache[n-1];
}
