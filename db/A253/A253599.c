/*
 * A253599  Centered square numbers (A001844) which are also centered
 *          heptagonal numbers (A069099).
 * https://oeis.org/A253599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+64514*a(n-2)-64514*a(n-3)-a(n-4)+a(n-5), a(1)=1,
 *            a(2)=841, a(3)=43513, a(4)=54236113, a(5)=2807177521
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n == 0)  /* out of range, but specification allows undefined */
        return 0;

    size_t idx = (size_t)(n - 1);  /* zero‑based index */

    if (cache == NULL) {
        /* initial allocation: at least 5 terms, and enough for first request */
        cache_cap = idx + 1 > 5 ? idx + 1 : 5;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)841;
        cache[2] = (seq_t)43513;
        cache[3] = (seq_t)54236113;
        cache[4] = (seq_t)2807177521;
        cache_len = 5;
    } else if (idx >= cache_len) {
        /* grow capacity if needed */
        size_t new_cap = cache_cap;
        while (new_cap <= idx)
            new_cap <<= 1;  /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;

        /* compute new terms up to idx */
        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t term = cache[i-1] + (seq_t)64514 * cache[i-2];
            term -= (seq_t)64514 * cache[i-3];
            term -= cache[i-4];
            term += cache[i-5];
            cache[i] = term;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
