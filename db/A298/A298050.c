/*
 * A298050  Number of nX3 0..1 arrays with every element equal to 1, 2, 4 or 6
 *          king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 45, u64 n <= 91, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Empirical recurrence for A298050:
 *
 *    a(1) = 1, a(2) = 7, a(3) = 15, a(4) = 19, a(5) = 23,
 *    a(6) = 34, a(7) = 63, a(8) = 96, a(9) = 147, a(10) = 233,
 *    a(11) = 368, a(12) = 588, a(13) = 933, a(14) = 1500,
 *    a(15) = 2404, a(16) = 3842, a(17) = 6157, a(18) = 9887,
 *    a(19) = 15907
 *
 *    For n > 19:
 *    a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) - 3*a(n-5) + 3*a(n-6) + 3*a(n-7)
 *           - a(n-8) + 2*a(n-9) + 6*a(n-10) - 8*a(n-11) - 3*a(n-12)
 *           - 4*a(n-13) - 2*a(n-14) + 4*a(n-15) + 8*a(n-16)
 *           - a(n-17) - 3*a(n-18)
 *
 *    The function uses memoisation; it is safe to be called with n
 *    values in any order (including non‑increasing).
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cached = 0; /* number of allocated elements */

    /* Base values (a(1) … a(19)) */
    static const seq_t base[19] = {
        1ULL,   7ULL,  15ULL,  19ULL,  23ULL,
        34ULL,  63ULL,  96ULL, 147ULL, 233ULL,
        368ULL, 588ULL, 933ULL,1500ULL,2404ULL,
        3842ULL,6157ULL,9887ULL,15907ULL
    };

    /* Off‑by‑one offset: n == 0 is undefined for this sequence */
    if (n < 1)
        return 0;

    /* Ensure the cache contains at least max(n, 19) entries */
    if (cached == 0) {
        size_t need = (size_t)n > 19 ? (size_t)n : 19;
        cache = (seq_t *) malloc(need * sizeof(seq_t));
        if (!cache) exit(1);          /* out of memory */
        cached = need;
        /* initialise base values */
        for (size_t i = 0; i < 19; ++i) cache[i] = base[i];
        for (size_t i = 19; i < cached; ++i) cache[i] = 0;
    } else if ((size_t)n > cached) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *) realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cached = new_cap;
        /* zero‑initialise the new entries */
        for (size_t i = 19; i < cached; ++i) cache[i] = 0;
    }

    /* If the requested value is already known, return it */
    if (cache[n - 1] != 0 || n <= 19)
        return cache[n - 1];

    /* Compute missing terms up to n using the empirical recurrence */
    /* 0‑based index: i corresponds to a(i+1) */
    for (size_t i = 19; i < (size_t)n; ++i) {
        seq_t v = 0;
        v += 3 * cache[i - 1];
        v -=          cache[i - 2];
        v -= 2 * cache[i - 3];
        v -= 3 * cache[i - 5];
        v += 3 * cache[i - 6];
        v += 3 * cache[i - 7];
        v -=          cache[i - 8];
        v += 2 * cache[i - 9];
        v += 6 * cache[i -10];
        v -= 8 * cache[i -11];
        v -= 3 * cache[i -12];
        v -= 4 * cache[i -13];
        v -= 2 * cache[i -14];
        v += 4 * cache[i -15];
        v += 8 * cache[i -16];
        v -=          cache[i -17];
        v -= 3 * cache[i -18];
        cache[i] = v;
    }

    return cache[n - 1];
}
