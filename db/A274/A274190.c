/*
 * A274190  Triangular array read by rows: g(n,k) = 1 for n >= 0; g(n,k) = 0
 *          if k > n; g(n,k) = g(n-1,k-1) + g(n-1,2k) for n > 0, k > 1.
 * https://oeis.org/A274190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 172, u16 n <= 529, u32 n <= 1831, u64 n <= 6904,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   g(0,0) = 1;               g(n,0) = 1  for n > 0
   g(n,k) = g(n-1,k-1) + g(n-1,2k)   for n > 0, k > 0
   a(n) is read by rows of the triangular array:
       let row = ⌊(√(8n+1)‑1)/2⌋,
       col  = n – row*(row+1)/2
   then a(n) = g(row,col) */
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
    /* Placement new needed only in C++ for class seq_t */
    #define PLACEHOLDEREVAL(ptr, type) new (ptr) type()
#else
    #define PLACEHOLDEREVAL(ptr, type) ((void)0)
#endif

/* Integer square root: floor(sqrt(v)) for unsigned 64‑bit v */
static idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * mid <= v) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

/* Cached triangular array of g(n,k) values, stored row by row
   (row 0 occupies index 0, row 1 occupies indices 1,2, etc.) */
static seq_t *cache = 0;
static idx_t cached_rows = 0;   /* number of rows currently stored (0..cached_rows-1) */
static size_t cached_terms = 0; /* number of elements currently allocated */

/* Ensure that rows 0..need_row are filled and cache holds at least
   cached_terms = (need_row+1)*(need_row+2)/2 elements. */
static void ensure_rows(idx_t need_row)
{
    if (cached_rows > need_row) return;

    /* Determine how many new terms we need to allocate */
    idx_t new_terms = 0;
    for (idx_t r = cached_rows; r <= need_row; ++r)
        new_terms += r + 1;

    size_t old_cap   = cached_terms;
    size_t new_cap   = old_cap + new_terms;

    /* Allocate a new array and copy existing elements */
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) abort();

    for (size_t i = 0; i < old_cap; ++i) {
        new_cache[i] = cache[i];
    }
    /* Construct new elements (C++ only; C integers need no construction) */
    for (size_t i = old_cap; i < new_cap; ++i) {
        PLACEHOLDEREVAL(&new_cache[i], seq_t);
    }

    /* Free old array */
    free(cache);
    cache      = new_cache;
    cached_terms = new_cap;

    /* Fill in the new rows */
    for (idx_t r = cached_rows; r <= need_row; ++r) {
        idx_t start = r * (r + 1) / 2;          /* index of g(r,0) */
        if (r == 0) {                          /* base row */
            cache[start] = (seq_t)1;
            continue;
        }
        idx_t prev_start = (r - 1) * r / 2;     /* start index of previous row */
        cache[start] = (seq_t)1;                /* g(r,0) = 1 */
        for (idx_t k = 1; k <= r; ++k) {
            seq_t val = cache[prev_start + (k - 1)];  /* g(r-1,k-1) */
            if ((idx_t)(2 * k) <= r - 1) {
                val += cache[prev_start + (2 * k)];   /* g(r-1,2k) */
            }
            cache[start + k] = val;          /* store g(r,k) */
        }
    }

    cached_rows = need_row + 1;
}

/* Public function required by the tests */
seq_t a(idx_t n)
{
    /* Find the row that contains a(n) */
    idx_t row = (isqrt_u64(8 * n + 1) - 1) / 2;

    ensure_rows(row);

    idx_t start = row * (row + 1) / 2;
    idx_t col   = n - start;
    return cache[start + col];
}
