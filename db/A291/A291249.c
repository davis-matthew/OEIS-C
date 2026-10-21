/*
 * A291249  p-INVERT of (0,1,0,1,0,1,...), where p(S) = 1 - S - S^2 - S^3 +
 *          S^4 + S^5.
 * https://oeis.org/A291249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A291249 – p‑INVERT of (0,1,0,1,0,1,…)
   Recurrence (n ≥ 11):
     a(n) = a(n-1) + 6·a(n-2) – 3·a(n-3) – 14·a(n-4) + 3·a(n-5)
            + 14·a(n-6) – 3·a(n-7) – 6·a(n-8) + a(n-9) + a(n-10)
   Initial values:
     a(0)=1, a(1)=2, a(2)=5, a(3)=10, a(4)=23, a(5)=47,
     a(6)=102, a(7)=214, a(8)=452, a(9)=955, a(10)=2003
   This implementation memoises all computed values in a
   dynamically‑resizing static array, so calls may appear in any order. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------- */
/* Main function                                                  */
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* ---- 1. Base values -------------------------------------- */
    static const seq_t base[11] = {
        (seq_t)1,   (seq_t)2,   (seq_t)5,   (seq_t)10,  (seq_t)23,
        (seq_t)47,  (seq_t)102, (seq_t)214, (seq_t)452, (seq_t)955,
        (seq_t)2003
    };

    /* ---- 2. Static memoisation storage ------------------------ */
    static seq_t *cache  = NULL;      /* array of a(i) values */
    static size_t cap    = 0;         /* allocated capacity */
    static idx_t  computed = (idx_t)-1; /* highest index that has been computed */

    /* ---- 3. Initialise cache on first call -------------------- */
    if (cache == NULL) {
        /* Allocate at least 11 elements (initial segment)           */
        cap = (size_t)(n + 1);
        if (cap < 11) cap = 11;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* failure – should not happen */

        /* Copy the known initial segment                 */
        for (idx_t i = 0; i <= 10; ++i) cache[i] = base[i];

        computed = 10; /* we have values 0 … 10 available            */
    }

    /* ---- 4. Grow the cache if the requested index is beyond      */
    /*         the current capacity                                 */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap : 1;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* failure – should not happen */
        cache = new_cache;
        cap   = new_cap;
    }

    /* ---- 5. Return value if already known ----------------------- */
    if (n <= computed) {
        return cache[n];
    }

    /* ---- 6. Compute missing terms up to n ---------------------- */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = cache[i - 1];
        pos += cache[i - 2] * (seq_t)6;
        pos += cache[i - 5] * (seq_t)3;
        pos += cache[i - 6] * (seq_t)14;
        pos += cache[i - 9];
        pos += cache[i -10];

        /* Negative contributions */
        seq_t neg = cache[i - 3] * (seq_t)3;
        neg += cache[i - 4] * (seq_t)14;
        neg += cache[i - 7] * (seq_t)3;
        neg += cache[i - 8] * (seq_t)6;

        cache[i] = pos - neg;   /* safe: pos ≥ neg for the recurrence    */
    }

    computed = n;   /* new highest index computed */
    return cache[n];
}
