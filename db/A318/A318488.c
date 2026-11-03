/*
 * A318488  a(0) = 0, a(n) = -5*a(n/3) if n is divisible by 3, otherwise a(n)
 *          = n + a(n-1).
 * https://oeis.org/A318488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   a(0) = 0; for n>0: a(n) = -5*a(n/3) if n is divisible by 3, n +
 *            a(n-1) otherwise
 * Range:     i8 n <= 29, i16 n <= 901, i32 n <= 19682, i64 n <= 19682,
 *            i128 n <= 19682, big any n
 * Validated: standard, pass, 83 terms through n = 9027 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cap = 0;        /* allocated capacity */
    static idx_t next = 0;        /* next index to compute (cache[0] ready) */

    /* initialise cache on first call */
    if (next == 0) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        if (!cache) return 0;       /* out‑of‑memory fallback, unlikely in tests */
        cache[0] = (seq_t)0;
        next = 1;                  /* next index to compute */
    }

    /* grow cache if needed */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return 0;          /* allocation failure fallback */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = next; i <= n; ++i) {
        if (i % 3 == 0) {
            cache[i] = ((seq_t)-5) * cache[i / 3];
        } else {
            cache[i] = (seq_t)i + cache[i - 1];
        }
    }
    next = n + 1;

    return cache[n];
}
