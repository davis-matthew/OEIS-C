/*
 * A277617  Lexicographically earliest positive sequence such that a(n+1)-a(n)
 *          is a square > 1 and no number occurs twice; a(1) = 1.
 * https://oeis.org/A277617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-13) - a(n-14) for n > 14, with initial
 *            values a(1)=1, a(2)=5, a(3)=9, a(4)=13, a(5)=4, a(6)=8, a(7)=12,
 *            a(8)=3, a(9)=7, a(10)=11, a(11)=2, a(12)=6, a(13)=10, a(14)=14
 * Range:     u8 n <= 249, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *seq = NULL;
    static size_t cap = 0;   /* capacity in elements */
    static size_t size = 0;  /* highest index computed (1‑based) */

    /* initialize cache if needed */
    if (!seq) {
        cap  = 16;
        seq  = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0;   /* allocation failure; behaviour undefined */
    }

    /* grow cache if n exceeds current capacity */
    if ((size_t)n >= cap) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        if (!seq) return 0;   /* allocation failure; behaviour undefined */
        cap = newcap;
    }

    /* compute missing terms up to n */
    if ((size_t)n > size) {
        /* initial values for indices 1..14 */
        static const seq_t init[15] = {
            0,   /* dummy for 0 */
            1, 5, 9, 13, 4, 8, 12, 3,
            7, 11, 2, 6, 10, 14
        };

        for (idx_t i = (idx_t)(size + 1); i <= n; ++i) {
            if (i <= 14) {
                seq[(size_t)i] = init[(size_t)i];
            } else {
                /* recurrence: a(i) = a(i-1) + a(i-13) - a(i-14) */
                seq[(size_t)i] =
                    seq[(size_t)(i - 1)] +
                    seq[(size_t)(i - 13)] -
                    seq[(size_t)(i - 14)];
            }
        }
        size = n;
    }

    return seq[(size_t)n];
}
