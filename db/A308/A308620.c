/*
 * A308620  Number of states in the evolutionary spatial prisoner's dilemma
 *          with n players.
 * https://oeis.org/A308620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   linear recurrence For n ≥ 12: a(n) = 2*a(n-1) – 2*a(n-3) +
 *            a(n-4) + 2*a(n-5) – 2*a(n-6) – a(n-10) Initial terms (n = 1 …
 *            11): 2, 2, 2, 3, 4, 6, 9, 13, 19, 28, 42 The function memoises
 *            results so that any call order runs in O(n)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle offset: a(1) is first term */
    if (n == 0) return 0;  /* undefined, but guard against 0 */

    /* static cache: cache[i] holds a(i), cache[0] is dummy */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored elements (including dummy) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* first 11 terms plus dummy 0 at index 0 */
    static const seq_t init[12] = {
        0, /* dummy for index 0 */
        2, 2, 2, 3, 4, 6, 9, 13, 19, 28, 42
    };

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 12;  /* enough for first 11 terms + dummy */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        for (idx_t i = 0; i < cache_cap; ++i) cache[i] = init[i];
        cache_len = 12;   /* last valid index is 11 */
    }

    /* extend cache if necessary */
    while (cache_len <= n) {
        /* ensure capacity */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= (size_t)cache_len) new_cap = (size_t)cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute next term using recurrence */
        /* indices used: n-1, n-3, n-4, n-5, n-6, n-10 (all exist for n >= 12) */
        seq_t next = 2 * cache[cache_len - 1]
                   - 2 * cache[cache_len - 3]
                   +       cache[cache_len - 4]
                   + 2 * cache[cache_len - 5]
                   - 2 * cache[cache_len - 6]
                   -       cache[cache_len -10];
        cache[cache_len++] = next;
    }

    return cache[n];
}
