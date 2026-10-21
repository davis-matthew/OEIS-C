/*
 * A266615  Total number of ON (black) cells after n iterations of the "Rule
 *          41" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A266615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-a(n-3)+a(n-4)-a(n-5)-a(n-6)+a(n-7) Initial
 *            values: a(0)=1, a(1)=1, a(2)=3, a(3)=6, a(4)=7, a(5)=15, a(6)=17
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* small indices handled by a static table */
    if (n < 7) {
        static const seq_t init[7] = {1, 1, 3, 6, 7, 15, 17};
        return init[n];
    }

    /* dynamic cache that grows as needed */
    static seq_t *cache   = NULL;   /* points to an array of seq_t values */
    static idx_t  cached  = 0;       /* number of elements currently cached */

    /* initialise cache if this is the first large call */
    if (cached == 0) {
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache) return 0;      /* out of memory – unlikely in tests */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 6;
        cache[4] = 7;
        cache[5] = 15;
        cache[6] = 17;
        cached = 7;                /* we now have values 0..6 */
    }

    /* extend the cache if necessary */
    if (n + 1 > cached) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;        /* out of memory – unlikely in tests */
        cache = tmp;

        /* compute new entries using the recurrence */
        for (idx_t i = cached; i <= n; ++i) {
            seq_t pos = cache[i - 1];
            pos += cache[i - 2];
            pos += cache[i - 4];
            pos += cache[i - 7];

            seq_t neg = cache[i - 3];
            neg += cache[i - 5];
            neg += cache[i - 6];

            cache[i] = pos - neg;   /* pos >= neg for all i >= 7 */
        }
        cached = n + 1;
    }

    return cache[n];
}
