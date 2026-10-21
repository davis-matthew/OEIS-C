/*
 * A274196  Triangular array read by rows: g(n,k) = 1 for n >= 0; g(n,k) = 0
 *          if k > n; g(n,k) = g(n-1,k-1) + g(n-1,4k) for n > 0, k > 1.
 * https://oeis.org/A274196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 596, u16 n <= 2017, u32 n <= 7261, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A274196
 * Triangular array read by rows: g(n,k) = 1 for n >= 0;
 * g(n,k) = 0 if k > n;
 * g(n,k) = g(n-1,k-1) + g(n-1,4k)   for n > 0, k > 1.
 *
 * a(n) is the element of the array when it is flattened row‑major
 * (row 0 first, then row 1, etc.).  This implementation builds
 * rows on demand and keeps them in a static cache so that any
 * calling order of a(idx_t) is supported.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined(__cplusplus)
#  include <new>
#  define ALLOC_ARRAY(type, n) new type[(size_t)(n)]
#  define REALLOC_ARRAY(ptr, type, new_n, old_n)                    \
     do {                                                          \
         type *tmp = new type[(size_t)(new_n)];                    \
         std::memcpy(tmp, ptr, (size_t)(old_n) * sizeof(type));    \
         delete[] ptr;                                             \
         ptr = tmp;                                                \
     } while (0)
#  define FREE_ARRAY(type, ptr) delete[] ptr
#else
#  define ALLOC_ARRAY(type, n) ((type*)malloc((size_t)(n) * sizeof(type)))
#  define REALLOC_ARRAY(ptr, type, new_n, old_n)                       \
     ptr = (type*)realloc(ptr, (size_t)(new_n) * sizeof(type))
#  define FREE_ARRAY(type, ptr) free(ptr)
#endif

seq_t a(idx_t n)
{
    /* Locate row r and column c of the n-th flattened element.
     * The cumulative count of elements up to row r-1 is r*(r+1)/2. */
    idx_t r = 0;
    idx_t prefix = 0;          /* elements in rows 0 .. r-1 */
    idx_t count  = 1;          /* elements in current row r (=r+1) */
    while (prefix + count <= n) {
        prefix += count;
        ++r;
        count = r + 1;
    }
    idx_t c = n - prefix;      /* column index within row r */

    /* Static cache of computed rows.  g[i] points to row i (size i+1). */
    static seq_t **g = NULL;
    static size_t max_row = 0;  /* last row that has been constructed */

    /* First call: allocate the array that will hold row pointers. */
    if (!g) {
        g = ALLOC_ARRAY(seq_t*, 1);
        g[0] = ALLOC_ARRAY(seq_t, 1);
        g[0][0] = (seq_t)1;    /* g(0,0)=1 */
        max_row = 0;
    }

    /* Build any missing rows up to row r. */
    if ((size_t)r > max_row) {
        size_t new_size = (size_t)r + 1;
        if (new_size > max_row + 1) {
            REALLOC_ARRAY(g, seq_t*, new_size, max_row + 1);
        }
        for (size_t i = max_row + 1; i <= (size_t)r; ++i) {
            size_t len = i + 1;
            seq_t *row = ALLOC_ARRAY(seq_t, len);
            /* first element is always 1 */
            row[0] = (seq_t)1;
            size_t prev_len = i;          /* length of row i-1 */
            for (size_t k = 1; k < len; ++k) {
                seq_t val = g[i - 1][k - 1];          /* g(i-1,k-1) */
                size_t idx4k = 4 * k;
                if (idx4k < prev_len)                /* g(i-1,4k) exists */
                    val += g[i - 1][idx4k];
                row[k] = val;
            }
            g[i] = row;
        }
        max_row = (size_t)r;
    }

    return g[(size_t)r][(size_t)c];
}
