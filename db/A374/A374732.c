/*
 * A374732  a(n) = a(n-1) + rotate(a(n-1), n-1 digits right) with a(1) = 1.
 * https://oeis.org/A374732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   digit_len, pow10, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374732
   Formula: a(n) = a(n-1) + rotate(a(n-1), n-1 digits right),  a(1) = 1
   • rotate(x, k) means cyclic right rotation of the decimal digits of x by k positions
   • The rotation amount is taken modulo the number of digits of x.
   • The implementation uses memoisation; the cache is thread‑local static data
     that grows as needed.
   • All arithmetic stays within seq_t; intermediate values are computed
     with seq_t operations only. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* cache[0] holds a(1) */
    static seq_t *cache      = NULL;   /* pointer to cached terms */
    static idx_t cache_filled = 0;     /* number of terms currently filled (>=1) */
    static size_t cache_cap   = 0;     /* allocated capacity */

    if (n == 0) return (seq_t)0;        /* undefined for n < 1, return 0 */

    /* -------------------------------------------------------------
       Grow the cache if required.  Allocate or reallocate using the
       helper functions that return seq_t pointers.
       ------------------------------------------------------------- */
    if ((idx_t)cache_cap < n) {
        if (cache == NULL) {
            cache = seq_alloc((size_t)n);         /* allocate n elements */
            cache_cap = (size_t)n;
        } else {
            cache = seq_realloc(cache, cache_cap, (size_t)n);
            cache_cap = (size_t)n;
        }
    }

    /* -------------------------------------------------------------
       Compute missing terms up to n.
       ------------------------------------------------------------- */
    for (idx_t idx = cache_filled + 1; idx <= n; ++idx) {
        if (idx == 1) {
            /* a(1) = 1 */
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t prev = cache[idx - 2];                /* a(idx-1) */
        int  d    = digit_len(prev);               /* decimal digits of a(idx-1) */

        /* rotation amount: (idx-1) mod d */
        idx_t shift = (idx - 1) % (idx_t)d;

        seq_t rotated;
        if (shift == 0) {
            rotated = prev;
        } else {
            seq_t pow10_shift = pow10(shift);               /* 10^shift   */
            seq_t low  = prev % pow10_shift;               /* last shift digits */
            seq_t high = prev / pow10_shift;               /* remaining digits */
            seq_t pow10_other = pow10((idx_t)(d - shift)); /* 10^(d-shift) */
            rotated = low * pow10_other + high;
        }

        cache[idx - 1] = prev + rotated;               /* a(idx) = a(idx-1)+rotated */
    }

    cache_filled = n;
    return cache[n - 1];
}
