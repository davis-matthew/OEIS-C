/*
 * A304298  Number of nX4 0..1 arrays with every element unequal to 1, 2, 5 or
 *          8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+5*a(n-3)-2*a(n-4)+2*a(n-5)-8*a(n-6) *
 *            -2*a(n-7)-a(n-8)-4*a(n-9)-a(n-10)-2*a(n-12)-a(n-13)-a(n-14)-a(n-15)
 *            * (n>19) * Initial values:
 *            a(1)=2,a(2)=1,a(3)=5,a(4)=4,a(5)=13,a(6)=30,a(7)=43,a(8)=129, *
 *            a(9)=245,a(10)=519,a(11)=1254,a(12)=2525,a(13)=5707,a(14)=12749,
 *            * a(15)=27205,...
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 58, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* current allocated size */
    static idx_t  max_n = 0;     /* largest index stored */

    /* Allocate or resize cache if needed */
    size_t need = (size_t)n + 1; /* index 0 unused; we need up to n inclusive */
    if (cap < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) { abort(); }          /* out of memory */
        cache = tmp;
        cap   = need;
    }

    /* Initial terms a(1) .. a(19) */
    static const seq_t init[20] = {
        0,                      /* a(0) unused */
        2,                      /* a(1) */
        1,                      /* a(2) */
        5,                      /* a(3) */
        4,                      /* a(4) */
        13,                     /* a(5) */
        30,                     /* a(6) */
        43,                     /* a(7) */
        129,                    /* a(8) */
        245,                    /* a(9) */
        519,                    /* a(10) */
        1254,                   /* a(11) */
        2525,                   /* a(12) */
        5707,                   /* a(13) */
        12749,                  /* a(14) */
        27205,                  /* a(15) */
        60962,                  /* a(16) */
        133639,                 /* a(17) */
        292125,                 /* a(18) */
        647249                  /* a(19) */
    };

    /* Fill missing initial terms up to n if they have not yet been stored */
    for (idx_t i = max_n + 1; i <= 19 && i <= n; ++i) {
        cache[i] = init[i];
    }

    /* Compute and store terms for n > 19 */
    if (n > 19) {
        idx_t start = (max_n >= 19) ? max_n + 1 : 20;
        for (idx_t i = start; i <= n; ++i) {
            /* Recurrence computation */
            seq_t val = 0;
            val += cache[i - 1];
            val += cache[i - 2];
            val += 5 * cache[i - 3];
            val -= 2 * cache[i - 4];
            val += 2 * cache[i - 5];
            val -= 8 * cache[i - 6];
            val -= 2 * cache[i - 7];
            val -= cache[i - 8];
            val -= 4 * cache[i - 9];
            val -= cache[i - 10];
            val -= 2 * cache[i - 12];
            val -= cache[i - 13];
            val -= cache[i - 14];
            val -= cache[i - 15];
            cache[i] = val;
        }
    }

    max_n = (n > max_n) ? n : max_n;   /* update largest stored index */
    return cache[n];
}
