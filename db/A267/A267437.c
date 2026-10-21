/*
 * A267437  A linear recurrence related to the elliptic curves y^2 = x^3
 *          -35*a^2*x - 98*a^3 with a = -1, -5, -6, -17, or -111.
 * https://oeis.org/A267437
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 29, u64 n <= 61, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A267437: a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 4*a(n-4)  (n ≥ 6)
 * Offset 2.  a(2)=11, a(3)=23, a(4)=67, a(5)=151, … */
#include <stdlib.h>

/* If the compiler supports 128‑bit integers, use them as the
 * intermediate type; otherwise fall back to an unsigned long long. */
#if defined(__SIZEOF_INT128__) || defined(__uint128_t)
  #define BIGINT_T __uint128_t
#else
  #define BIGINT_T unsigned long long
#endif

seq_t a(idx_t n) {
    /* Undefined for indices below the offset. */
    if (n < 2) return (seq_t)0;

    /* Static cache and bookkeeping. */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* allocated size (index+1) */
    static idx_t last_filled = 1; /* greatest index that has a known value */

    /* Allocate / grow the cache as needed. */
    if (cache == NULL) {
        /* First allocation. */
        cap = (size_t)n + 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */
    } else if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Initialise the known initial terms if they are not yet present. */
    if (cap > 2) cache[2] = (seq_t)11;
    if (cap > 3) cache[3] = (seq_t)23;
    if (cap > 4) cache[4] = (seq_t)67;
    if (cap > 5) cache[5] = (seq_t)151;

    /* Update last_filled if the cache was just resized. */
    if (last_filled < 2) {
        /* After the first allocation we now have values up to min(cap-1,5). */
        last_filled = (cap-1 >= 5) ? 5
                    : ((cap-1 >= 2) ? (cap-1) : 1);
    }

    /* If we already computed up to n, simply return the stored value. */
    if (n <= last_filled) return cache[n];

    /* Compute missing terms using the recurrence. */
    for (idx_t i = last_filled + 1; i <= n; ++i) {
        if (i < 6) continue;  /* initial values already set */
        cache[i] = (seq_t)(
            (BIGINT_T)4 * cache[i-1] -
            (BIGINT_T)7 * cache[i-2] +
            (BIGINT_T)8 * cache[i-3] -
            (BIGINT_T)4 * cache[i-4]
        );
    }
    last_filled = n;
    return cache[n];
}
