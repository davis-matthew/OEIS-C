/*
 * A260064  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00000011 or 00000111.
 * https://oeis.org/A260064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 6*a(n-3) + 4*a(n-4) * - 10*a(n-5) -
 *            13*a(n-6) - 12*a(n-7) - 9*a(n-8) * + 7*a(n-9) + 6*a(n-10) +
 *            5*a(n-11) for n > 11 * Initial values: a(1)=91, a(2)=214,
 *            a(3)=594, a(4)=1214, a(5)=2367, * a(6)=5776, a(7)=13366,
 *            a(8)=28064, a(9)=63014, * a(10)=147105, a(11)=325915. * The f...
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 22, u64 n <= 49, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static const seq_t init[12] = {
    /*0*/ (seq_t)0,
    /*1*/ (seq_t)91,
    /*2*/ (seq_t)214,
    /*3*/ (seq_t)594,
    /*4*/ (seq_t)1214,
    /*5*/ (seq_t)2367,
    /*6*/ (seq_t)5776,
    /*7*/ (seq_t)13366,
    /*8*/ (seq_t)28064,
    /*9*/ (seq_t)63014,
    /*10*/ (seq_t)147105,
    /*11*/ (seq_t)325915
};

seq_t a(idx_t n)
{
    /* n = 0 is outside the offset; return 0. */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;      /* cache[0] unused, cache[1] .. cache[N] store values */
    static size_t cache_size = 0;    /* allocated size (index + 1) */
    static idx_t  computed_max = 0;  /* largest index for which cache[i] is valid */

    /* Allocate (or grow) the cache to hold up to index n. */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)            /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise early entries with the known initial values if they are not yet set. */
    if (computed_max < 11) {
        for (idx_t i = computed_max + 1; i <= 11 && i < (idx_t)cache_size; ++i)
            cache[i] = init[i];
        computed_max = (idx_t)(cache_size - 1 <= 11 ? cache_size - 1 : 11);
    }

    /* If the requested value is already computed, return it immediately. */
    if (n <= computed_max)
        return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* pos   = a(i-1)+a(i-2)+6*a(i-3)+4*a(i-4)+7*a(i-9)+6*a(i-10)+5*a(i-11)  */
        /* neg   = 10*a(i-5)+13*a(i-6)+12*a(i-7)+9*a(i-8)                          */
        /* term  = pos - neg                                                         */
        seq_t pos = cache[i-1] + cache[i-2];
        pos += (seq_t)6 * cache[i-3];
        pos += (seq_t)4 * cache[i-4];
        pos += (seq_t)7 * cache[i-9];
        pos += (seq_t)6 * cache[i-10];
        pos += (seq_t)5 * cache[i-11];

        seq_t neg = (seq_t)10 * cache[i-5];
        neg += (seq_t)13 * cache[i-6];
        neg += (seq_t)12 * cache[i-7];
        neg += (seq_t)9  * cache[i-8];

        cache[i] = pos - neg;   /* safe: pos ≥ neg for all n≥12 */
    }

    computed_max = n;
    return cache[n];
}
