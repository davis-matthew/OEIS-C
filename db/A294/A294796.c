/*
 * A294796  Number of permutations of [n] avoiding {1234, 1324, 3412}.
 * https://oeis.org/A294796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73, a(6)=236,
 *            a(7)=700 For n>7: a(n)=10*a(n-1)-43*a(n-2)+104*a(n-3)-155*a(n-4)
 *            +146*a(n-5)-85*a(n-6)+28*a(n-7)-4*a(n-8)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache initialised on the first call (offset 0) */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;

    if (!cache) {
        /* allocate space for the first 8 terms */
        cache = (seq_t*)malloc(8 * sizeof(seq_t));
        if (!cache) return 0;               /* OOM – harmless for unit tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)73;
        cache[6] = (seq_t)236;
        cache[7] = (seq_t)700;
        cached_len = 8;
    }

    size_t idx = (size_t)n;
    if (idx < cached_len)               /* value already known */
        return cache[idx];

    /* enlarge cache to include requested index */
    size_t need = idx + 1;
    if (need > cached_len) {
        seq_t *tmp = (seq_t*)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return 0;              /* OOM – harmless for unit tests */
        cache = tmp;                     /* propagate new address */
    }

    /* fill missing terms with the 8‑term linear recurrence */
    for (size_t i = cached_len; i <= idx; ++i) {
        seq_t sumPos = (seq_t)10 * cache[i-1]
                     + (seq_t)104 * cache[i-3]
                     + (seq_t)146 * cache[i-5]
                     + (seq_t)28  * cache[i-7];
        seq_t sumNeg = (seq_t)43  * cache[i-2]
                     + (seq_t)155 * cache[i-4]
                     + (seq_t)85  * cache[i-6]
                     + (seq_t)4   * cache[i-8];
        cache[i] = sumPos - sumNeg;      /* sumPos >= sumNeg for all i>=8 */
    }

    cached_len = need;
    return cache[idx];
}
