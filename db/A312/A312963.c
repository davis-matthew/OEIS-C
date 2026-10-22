/*
 * A312963  Coordination sequence Gal.6.153.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A312963 – Coordination sequence Gal.6.153.2
 * Recurrence (for n > 10):
 *   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) - a(n-10)
 *
 * Initial values:
 *   a(0)=1, a(1)=4, a(2)=9, a(3)=13, a(4)=18, a(5)=24,
 *   a(6)=29, a(7)=34, a(8)=40, a(9)=45, a(10)=49
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;        /* allocated slots (0 … capacity-1) */
    static size_t last = SIZE_MAX;     /* highest index computed so far */

    /* Known initial terms for n = 0 … 10 */
    static const seq_t init_vals[11] = {
        (seq_t)1, (seq_t)4, (seq_t)9, (seq_t)13, (seq_t)18,
        (seq_t)24, (seq_t)29, (seq_t)34, (seq_t)40, (seq_t)45, (seq_t)49
    };

    /* Initialise cache on first call */
    if (!cache) {
        capacity = 11;                /* indices 0 … 10 */
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        for (size_t i = 0; i < 11; ++i) {
            cache[i] = init_vals[i];
        }
        last = 10;                    /* all initial terms known */
    }

    /* Grow cache if required */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity ? capacity * 2 : 16;
        while ((size_t)n >= new_cap) new_cap *= 2;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        capacity = new_cap;
    }

    /* Compute missing terms if n is beyond the last computed index */
    if (last < (size_t)n) {
        /* Start from the first index that hasn't been computed yet */
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            /* Recurrence: a(i) = a(i-1)+a(i-3)-a(i-4)-a(i-6)+a(i-7)+a(i-9)-a(i-10) */
            seq_t val = cache[i - 1];
            val += cache[i - 3];
            val -= cache[i - 4];
            val -= cache[i - 6];
            val += cache[i - 7];
            val += cache[i - 9];
            val -= cache[i - 10];
            cache[i] = val;
        }
        last = (size_t)n;
    }

    return cache[(size_t)n];
}
