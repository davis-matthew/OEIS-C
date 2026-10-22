/*
 * A309496  a(1) = 1, a(2) = 3, a(3) = a(4) = a(6) = 6, a(5) = 2, a(7) = 4;
 *          a(n) = a(n-a(n-4)) + a(n-a(n-5)) for n > 7.
 * https://oeis.org/A309496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=3, a(3)=6, a(4)=6, a(5)=2, a(6)=6, a(7)=4
 *            a(n)=a(n-a(n-4))+a(n-a(n-5)) for n>7 (indices start at 1, as in
 *            OEIS)
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

/* maximum n we are prepared to handle; the sequence grows roughly
   linearly and is always positive, so a small static cache suffices. */
#define CACHE_MAX 200000

static seq_t cache[CACHE_MAX + 1]; /* static storage guarantees zero init. */
static idx_t computed_up_to = 0;   /* highest index for which cache[i] is known */

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; a(0) is undefined */
        return 0;

    if (n > CACHE_MAX)          /* out of range for static cache */
        return 0;               /* or handle as error if desired */

    /* initialise the seven first terms once */
    if (computed_up_to < 7) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)6;
        cache[5] = (seq_t)2;
        cache[6] = (seq_t)6;
        cache[7] = (seq_t)4;
        computed_up_to = 7;
    }

    /* if we already know the term, return it immediately */
    if (n <= computed_up_to)
        return cache[n];

    /* compute sequentially up to n, reusing previously computed values */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t a_i4 = cache[i - 4];
        seq_t a_i5 = cache[i - 5];

        /* indices are given by i - a(i-4) and i - a(i-5);
           perform the subtraction in seq_t to avoid overflow,
           then cast the result to idx_t for array access.   */
        seq_t diff1 = (seq_t)i - a_i4;
        seq_t diff2 = (seq_t)i - a_i5;
        idx_t idx1 = (idx_t)diff1;
        idx_t idx2 = (idx_t)diff2;

        cache[i] = cache[idx1] + cache[idx2];
    }
    computed_up_to = n;
    return cache[n];
}
