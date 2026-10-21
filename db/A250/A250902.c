/*
 * A250902  Number of (4+1) X (n+1) 0..2 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 9th‑order linear recurrence a(n) = 14*a(n-1)
 *            - 85*a(n-2) + 294*a(n-3) - 639*a(n-4) + 906*a(n-5) - 839*a(n-6)
 *            + 490*a(n-7) - 164*a(n-8) + 24*a(n-9); Initial values:
 *            a(1)=1333, a(2)=4750, a(3)=14196, a(4)=39003, a(5)=102789,
 *            a(6)=265546, a(7)=680504, a(8)=1742387, a(9)=4479925
 * Range:     u8 none, u16 n <= 4, u32 n <= 15, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for n < 1; safe return */

    /* Static cache of previously computed terms (1‑based indexing) */
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of terms stored (>=0) */
    static size_t cap    = 0;   /* current allocation size */

    /* Grow cache if needed */
    if ((size_t)n > cap) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Predefined initial values a(1)..a(9) */
    static const seq_t init[9] = {
        (seq_t)1333,    (seq_t)4750,    (seq_t)14196,   (seq_t)39003,
        (seq_t)102789,  (seq_t)265546,  (seq_t)680504,  (seq_t)1742387,
        (seq_t)4479925
    };

    /* Compute terms up to n */
    size_t i;
    for (i = cached; i < (size_t)n; ++i) {
        if (i < 9) {
            cache[i] = init[i];
        } else {
            seq_t pos = 0, neg = 0;
            pos += cache[i-1] * 14;
            pos += cache[i-3] * 294;
            pos += cache[i-5] * 906;
            pos += cache[i-7] * 490;
            pos += cache[i-9] * 24;

            neg += cache[i-2] * 85;
            neg += cache[i-4] * 639;
            neg += cache[i-6] * 839;
            neg += cache[i-8] * 164;

            cache[i] = pos - neg;
        }
    }
    cached = (size_t)n;
    return cache[n-1];
}
