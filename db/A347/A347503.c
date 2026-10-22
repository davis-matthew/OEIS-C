/*
 * A347503  Number of dominating sets in the n-dipyramidal graph.
 * https://oeis.org/A347503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) - a(n-4) + 2*a(n-5) Initial values
 *            (offset 1): a(1)=5, a(2)=13, a(3)=29, a(4)=57, a(5)=115
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Explicitly handled offset: a(1) is the first term. */
    if (n == 0) return 0;          /* behaviour for indices < 1 is irrelevant */

    /* Initial five values */
    static const seq_t base[6] = {0, 5, 13, 29, 57, 115};

    /* Cache to allow arbitrary order of calls.  The size 10001 accommodates
       all n up to the expected limit (~10 000).  The array is statically
       allocated to avoid constructor issues in the C build. */
    enum { CACHE_MAX = 10001 };
    static seq_t cache[CACHE_MAX] = {0};
    static idx_t cache_len = 0;    /* highest index computed so far */

    /* Keep the cache up to date. */
    if (n > cache_len) {
        /* Extend only within bounds.  If a larger n is requested we simply
           compute on the fly without caching beyond the array size. */
        idx_t limit = n < CACHE_MAX ? n : CACHE_MAX - 1;
        for (idx_t i = cache_len + 1; i <= limit; ++i) {
            if (i <= 5) {
                cache[i] = base[i];
            } else {
                /* recurrence: a(i) = 4*a(i-1) - 4*a(i-2) - a(i-4) + 2*a(i-5) */
                seq_t val = 4 * cache[i - 1];
                val -= 4 * cache[i - 2];
                val -= cache[i - 4];
                val += 2 * cache[i - 5];
                cache[i] = val;
            }
        }
        if (limit > cache_len) cache_len = limit;
    }

    /* If n exceeds the static cache size, compute it on demand using the
       recurrence but without caching the result.  For the expected
       n≤10000 this path is never taken. */
    if (n > CACHE_MAX - 1) {
        /* Use a small rolling buffer of the last five values. */
        seq_t vals[5] = {0, 0, 0, 0, 0};
        /* Initialise the buffer with the known first five terms (modulo 5). */
        for (idx_t i = 1; i <= 5 && i <= n; ++i) {
            vals[(i - 1) % 5] = base[i];
        }
        for (idx_t i = 6; i <= n; ++i) {
            seq_t next = 4 * vals[(i - 2) % 5];
            next -= 4 * vals[(i - 3) % 5];
            next -= vals[(i - 5) % 5];
            next += 2 * vals[(i - 6) % 5];
            vals[(i - 1) % 5] = next;
        }
        return vals[(n - 1) % 5];
    }

    return cache[n];
}
