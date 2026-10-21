/*
 * A261375  Number of (n+2)X(3+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00001011 00010101 or 01010101.
 * https://oeis.org/A261375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 11*a(n-4) + 13*a(n-6) - 2*a(n-8) * - 2*a(n-10) +
 *            4*a(n-12) for n>17; * initial values: a(1)=60, a(2)=82,
 *            a(3)=202, a(4)=424, a(5)=869, * a(6)=1714, a(7)=3778, a(8)=7462,
 *            a(9)=15888, a(10)=31618, a(11)=68245, * a(12)=135058,
 *            a(13)=290195, a(14)=575938, a(15)=1238957, * a(16)=245595...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 26, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cached values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static idx_t computed_max = 0;   /* highest index already computed */

    /* base values up to n=17 */
    static const seq_t basevals[18] = {
        0,          /* 0 – unused */
        60,         /* a(1) */
        82,         /* a(2) */
        202,        /* a(3) */
        424,        /* a(4) */
        869,        /* a(5) */
        1714,       /* a(6) */
        3778,       /* a(7) */
        7462,       /* a(8) */
        15888,      /* a(9) */
        31618,      /* a(10) */
        68245,      /* a(11) */
        135058,     /* a(12) */
        290195,     /* a(13) */
        575938,     /* a(14) */
        1238957,    /* a(15) */
        2455954,    /* a(16) */
        5282429     /* a(17) */
    };

    /* ensure cache is large enough for n */
    size_t need = (size_t)(n > 17 ? n + 1 : 18);  /* +1 to allow 1‑based indexing */
    if (cache_size < need) {
        /* allocate or grow */
        seq_t *new_cache = (seq_t *)realloc(cache, (need + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failed – abort (sequence build cannot continue) */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_size = need;
    }

    /* initialize base terms if not already done */
    if (computed_max < 17) {
        for (idx_t i = computed_max + 1; i <= 17 && i <= (idx_t)cache_size; ++i) {
            cache[i] = basevals[i];
        }
        computed_max = 17;
    }

    /* compute terms up to n using the recurrence if needed */
    if (n > computed_max) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            /* indices are always >=1 because i > 17 */
            cache[i] = cache[i-2]
                     + 11 * cache[i-4]
                     + 13 * cache[i-6]
                     -  2 * cache[i-8]
                     -  2 * cache[i-10]
                     +  4 * cache[i-12];
        }
        computed_max = n;
    }

    return cache[n];
}
