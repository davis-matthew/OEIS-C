/*
 * A300170  Number of nX3 0..1 arrays with every element equal to 2, 3, 4, 5,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A300170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 6*a(n-2) - 22*a(n-3) - 38*a(n-4) + 20*a(n-5) +
 *            46*a(n-6) + 3*a(n-7) + 5*a(n-8) + 13*a(n-9) - 11*a(n-10) -
 *            4*a(n-11) for n>13 Initial values: a(1)=0, a(2)=3, a(3)=9,
 *            a(4)=37, a(5)=192, a(6)=932, a(7)=4712, a(8)=23795, a(9)=120610,
 *            a(10)=612315, a(11)=3110338, a(12)=15805917, a(13...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for already computed values */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;
    static idx_t   last  = 0;         /* highest index whose value is known */

    if (n == 0) return (seq_t)0;     /* defined only for n ≥ 1 */

    /* make sure the cache is large enough */
    if ((size_t)n >= cap) {
        size_t new_cap = (cap == 0) ? 8 : cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        /* initialise newly allocated part to zero */
        memset(cache + cap, 0, (new_cap - cap) * sizeof(seq_t));
        cap = new_cap;
    }

    /* fill missing entries up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 13) {               /* first 13 terms are known */
            static const seq_t init[14] = {
                0,                  /* dummy index 0 */
                0, 3, 9, 37, 192, 932,
                4712, 23795, 120610, 612315,
                3110338, 15805917, 80334285
            };
            cache[i] = init[i];
        } else {
            /* recurrence uses the last 11 values */
            static const int coeff[11] = {
                5,  6, -22, -38, 20, 46, 3, 5, 13, -11, -4
            };
            seq_t pos = 0, neg = 0;
            for (int k = 0; k < 11; ++k) {
                seq_t val = cache[i - 1 - k];
                int    c   = coeff[k];
                if (c >= 0)
                    pos += val * (seq_t)c;
                else
                    neg += val * (seq_t)(-c);
            }
            cache[i] = pos - neg;     /* pos ≥ neg by definition of the sequence */
        }
    }
    last = n;
    return cache[n];
}
