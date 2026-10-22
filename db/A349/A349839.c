/*
 * A349839  Triangle T(n,k) built by placing all ones on the left edge,
 *          [1,0,0,0] repeated on the right edge, and filling the body using
 *          the Pascal recurrence T(n,k) = T(n-1,k) + T(n-1,k-1).
 * https://oeis.org/A349839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) of OEIS A349839 flattened into a single sequence.
 * T(n,0) = 1.
 * T(n,n) = 1 if n mod 4 == 0, else 0.
 * For 0<k<n:  T(n,k) = T(n-1,k) + T(n-1,k-1).
 * The function a(n) returns the n‑th element of the flattened triangle.
 *
 * The implementation builds rows on demand, caching all values in a
 * contiguous array.  It uses a single temporary array to generate each
 * row in reverse order so that the previous row’s values are retained.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <string.h>   /* memcpy */

/* Cached flattened triangle */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of elements stored */

/* Temporary array holding the most recently computed row */
static seq_t *prev_row   = NULL;
static size_t prev_len   = 0;   /* capacity of prev_row */
static idx_t   last_row  = 0;   /* index of the last computed row */

/* Ensure the cached array has room for at least `needed` elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Ensure the temporary row array can hold `needed` entries. */
static void ensure_prev_cap(size_t needed)
{
    if (needed > prev_len) {
        seq_t *tmp = (seq_t*)realloc(prev_row, needed * sizeof(seq_t));
        if (!tmp) abort();
        prev_row = tmp;
        prev_len = needed;
    }
}

seq_t a(idx_t n)
{
    /* Lazy initialization on the first call. */
    if (!cache) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 1;
        cache_len = 1;

        prev_len = 1;
        prev_row = (seq_t*)malloc(prev_len * sizeof(seq_t));
        if (!prev_row) abort();
        prev_row[0] = 1;
        last_row = 0;
    }

    /* Generate rows until the cache covers index `n`. */
    while (cache_len <= (size_t)n) {
        idx_t r   = last_row + 1;
        size_t len = (size_t)(r + 1);

        ensure_prev_cap(len);

        /* Left edge */
        prev_row[0] = 1;

        /* Right edge */
        prev_row[len-1] = ((r & 3) == 0) ? 1 : 0;

        /* Interior entries computed in reverse to preserve previous values. */
        for (size_t k = len-1; k > 1; --k) {
            prev_row[k-1] = prev_row[k-1] + prev_row[k-2];
        }

        /* Append the new row to the flattened cache. */
        ensure_cache_cap(cache_len + len);
        memcpy(cache + cache_len, prev_row, len * sizeof(seq_t));
        cache_len += len;

        last_row = r;
    }

    return cache[(size_t)n];
}
