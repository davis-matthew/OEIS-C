/*
 * A298890  Number of nX3 0..1 arrays with every element equal to 2, 3, 5, 6
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = 4*a(n-1) - a(n-2)
 *            - 5*a(n-3) -10*a(n-4) +11*a(n-5) +15*a(n-6) +2*a(n-7) *
 *            -5*a(n-8) -33*a(n-9) -20*a(n-10) +47*a(n-11) +36*a(n-12)
 *            +0*a(n-13) * +4*a(n-14) -12*a(n-15) -2*a(n-16) +2*a(n-17) * with
 *            initial values a(1)=0, a(2)=3, a(3)=1, a(4)=4, a(5)=4,...
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 25, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MAX_INITIAL 18
static const seq_t INIT_VALUES[MAX_INITIAL] = {
    0, 3, 1, 4, 4, 11, 26, 66, 171, 462,
    1248, 3419, 9450, 26334, 73697, 206960, 582316, 1640549
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n <= MAX_INITIAL) return INIT_VALUES[n-1];

    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* first time allocate and initialise the cache with the known values */
    if (!cache) {
        cache = (seq_t*)malloc(MAX_INITIAL * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure – should not happen */
        for (size_t i = 0; i < MAX_INITIAL; ++i)
            cache[i] = INIT_VALUES[i];
        cap = MAX_INITIAL;
    }

    /* grow the cache if higher values are needed */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure – should not happen */
        cache = tmp;

        /* offsets and coefficients of the recurrence */
        static const int offs[17]   = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 };
        static const int coeffs[17] = { 4, -1, -5, -10, 11, 15, 2, -5, -33, -20, 47, 36, 0, 4, -12, -2, 2 };

        for (size_t i = cap; i < new_cap; ++i) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (int k = 0; k < 17; ++k) {
                int idx = (int)i - offs[k];
                seq_t term = cache[idx];
                if (coeffs[k] >= 0)
                    pos += term * (seq_t)coeffs[k];
                else
                    neg += term * (seq_t)(-coeffs[k]);
            }
            cache[i] = pos - neg;      /* pos >= neg for all terms of this sequence */
        }
        cap = new_cap;
    }

    return cache[n-1];
}
