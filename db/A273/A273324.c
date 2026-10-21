/*
 * A273324  Integers m such that m^2 + 3 is the sum of 4 but no fewer nonzero
 *          squares.
 * https://oeis.org/A273324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest m such that m^2+3 is a sum of 4 nonzero squares
 *            but not fewer Equivalent detection: accept m if m^2+3 is of the
 *            form 4^a*(8b+7) (Legendre's 3-square theorem)
 * Range:     u8 n <= 96, u16 n <= 9952, u32 n <= 9952, u64 n <= 9952,
 *            u128 n <= 9952, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* cached a(k) values */
    static size_t cap = 0;               /* allocated capacity */
    static size_t len = 0;               /* number of stored terms */
    static idx_t last_m = 0;             /* last m processed */

    /* expand cache until we have at least n terms */
    while (len < n) {
        last_m++;                                /* candidate m */
        seq_t N = (seq_t)last_m * (seq_t)last_m + 3;   /* m^2 + 3 */

        seq_t t = N;
        /* remove all factors of 4 */
        while (t % 4 == 0)
            t /= 4;
        /* test for 8b+7 form */
        if (t % 8 == 7) {
            /* store m in cache */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = (seq_t)last_m;
        }
    }
    return cache[n - 1];
}
