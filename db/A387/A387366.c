/*
 * A387366  Expansion of 1/(1 - 10*x + x^2)^(3/2).
 * https://oeis.org/A387366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A387366 (OEIS)
   Formula:  a(0) = 1,  a(1) = 15,
           for n > 1:  n*a(n) = 5*(2*n+1)*a(n-1) – (n+1)*a(n-2)
   The function memoises previously computed terms so that any call
   order is supported.  It uses 128‑bit arithmetic (when available) for
   64‑bit unsigned seq_t to avoid overflow before the final division.
   The code is valid C11 and C++17. */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static seq_t *cache = NULL;   /* array of computed a(n) */
    static size_t cap   = 0;      /* allocated capacity of cache */
    static size_t len   = 0;      /* number of entries actually stored (max index + 1) */

    /* ----- allocate storage enough for index n ----------------- */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* ----- initialise first terms -------------------------------- */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }
    if (len == 1 && n >= 1) {
        cache[1] = (seq_t)15;
        len = 2;
    }

    /* ----- compute any missing terms up to n ---------------------- */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i < 2) continue;                      /* a(0) and a(1) already set */
        idx_t idx = (idx_t)i;                     /* current index */

#ifdef __cplusplus
        /* C++ build – seq_t may be arbitrary‑precision.  Regular
           arithmetic works without overflow. */
        seq_t term = ( (seq_t)5 * seq_t((idx << 1) + 1) * cache[i-1] )
                    - ( (seq_t)(idx + 1) * cache[i-2] );
        cache[i]   = term / idx;
#else
        /* C build – use unsigned __int128 when available to
           avoid overflow for 64‑bit seq_t. */
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
        unsigned __int128 two_n_plus1 = (unsigned __int128)idx * 2 + 1;
        unsigned __int128 numerator  = (unsigned __int128)5 * two_n_plus1 * cache[i-1];
        numerator -= (unsigned __int128)(idx + 1) * cache[i-2];
        cache[i]  = (seq_t)(numerator / idx);
#else
        /* If __int128 not available, rely on native seq_t arithmetic.
           This may overflow early for 64‑bit seq_t, but the values
           will still be correct until they fit in seq_t. */
        seq_t term = ( (seq_t)5 * seq_t((idx << 1) + 1) * cache[i-1] )
                     - ( (seq_t)(idx + 1) * cache[i-2] );
        cache[i]   = term / idx;
#endif
#endif
    }

    len = (size_t)n + 1;      /* we have stored up to index n */
    return cache[n];
}
