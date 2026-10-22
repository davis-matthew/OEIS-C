/*
 * A346458  Numbers with hexadecimal representation A, AB, ABC, ..., ABCDEFA,
 *          ABCDEFAB, ...
 * https://oeis.org/A346458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 16*a(n-1) + 10 + ((n-1) mod 6), a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Sequence values are stored in a static dynamic array.
   The array holds terms a(1)..a(cache_len). */
static seq_t *cache = NULL;   /* array of values */
static size_t cache_len = 0;  /* number of terms stored (>= 0) */

/* Compute the nth term of A346458, with n >= 1.
   Calls may be in any order; results are cached. */
seq_t a(idx_t n)
{
    /* n is 1-indexed.  Guard against n==0 (offset = 1). */
    if (n == 0) return 0;

    /* Allocate the cache if it does not exist yet. */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) return 0;           /* allocation failed */
        cache[0] = (seq_t)10;                  /* a(1) */
    }

    /* If the requested term is not yet computed, extend the cache. */
    if ((size_t)n > cache_len) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (tmp == NULL) return 0;            /* allocation failed */
        cache = tmp;

        /* Compute a(cache_len+1) .. a(n) using the recurrence. */
        for (idx_t i = cache_len; i < n; ++i) {
            /* i is 0-based index of the current term (a(i+1)). */
            idx_t k = i;                     /* k = n-1 for a(n) */
            seq_t prev = cache[i - 1];       /* a(k)  */
            seq_t term = ((seq_t)16) * prev + (seq_t)10
                          + (seq_t)((k) % 6);
            cache[i] = term;                 /* a(k+1) */
        }
        cache_len = new_len;
    }

    return cache[n - 1];                      /* a(n) */
}
