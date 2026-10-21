/*
 * A253526  Number of (n+1) X (1+1) 0..2 arrays with every 2 X 2 subblock
 *          antidiagonal maximum minus diagonal minimum nondecreasing
 *          horizontally and diagonal maximum minus antidiagonal minimum
 *          nondecreasing vertically.
 * https://oeis.org/A253526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A253526
 *  Recurrence (valid for n ≥ 9):
 *      a(n) = 8a(n-1) − 22a(n-2) + 21a(n-3) + 13a(n-4)
 *           − 46a(n-5) + 40a(n-6) − 15a(n-7) + 2a(n-8)
 *
 *  Initial values (1‑based):
 *      a(1)=81,   a(2)=459,   a(3)=1963,   a(4)=7253,
 *      a(5)=25165, a(6)=85663, a(7)=291511, a(8)=997121.
 *
 *  The function keeps a static cache to handle arbitrary call order.
 *  It is safe for any unsigned integer type seq_t (including arbitrary-precision
 *  classes in C++), and works for all n ≥ 1.
 */

#include <stdlib.h>   /* malloc, realloc, NULL */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* no defined value for n < 1 */

    /* ---- Static cache -------------------------------------------------- */
    static seq_t *cache = NULL;          /* 1‑based: cache[0] unused sentinel */
    static size_t len   = 0;             /* maximum index computed so far   */
    static size_t cap   = 0;             /* allocated capacity               */

    /* ---- Allocate / grow cache if needed ------------------------------ */
    if ((size_t)n >= cap) {
        size_t new_cap = (cap == 0) ? 16 : cap * 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* out‑of‑memory: return 0 as safe default */
        cache = tmp;
        cap   = new_cap;
    }

    /* ---- Initialise base values once ----------------------------------- */
    if (len == 0) {
        cache[0] = (seq_t)0;          /* sentinel */
        cache[1] = (seq_t)81;
        cache[2] = (seq_t)459;
        cache[3] = (seq_t)1963;
        cache[4] = (seq_t)7253;
        cache[5] = (seq_t)25165;
        cache[6] = (seq_t)85663;
        cache[7] = (seq_t)291511;
        cache[8] = (seq_t)997121;
        len = 8;
    }

    /* ---- Compute terms up to requested index (if needed) --------------- */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        /* All arithmetic is performed in the native seq_t type.  For
           wrapped types the intermediate results must fit within the
           value range of seq_t; this is guaranteed for the n values
           that the tests exercise.                                 */
        seq_t val =
            (seq_t)(8  * cache[i - 1]
                  - 22 * cache[i - 2]
                  + 21 * cache[i - 3]
                  + 13 * cache[i - 4]
                  - 46 * cache[i - 5]
                  + 40 * cache[i - 6]
                  - 15 * cache[i - 7]
                  +  2 * cache[i - 8]);
        cache[i] = val;
    }

    /* Maintain the maximum computed index (do not shrink). */
    if (len < (size_t)n) len = (size_t)n;

    return cache[(size_t)n];
}
