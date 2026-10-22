/*
 * A305336  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1)+5*a(n-2)+a(n-3)-11*a(n-5)-24*a(n-6)+5*a(n-7)
                     +17*a(n-8)-5*a(n-9)+6*a(n-10)+14*a(n-11)-5*a(n-12)
                     -10*a(n-13)+2*a(n-14)+4*a(n-15)  for n>19
   a(1)=8, a(2)=9, a(3)=26, a(4)=81, a(5)=267, a(6)=938, a(7)=3186,
   a(8)=10840, a(9)=37115, a(10)=127093, a(11)=434701, a(12)=1487032,
   a(13)=5088052, a(14)=17408881, a(15)=59562461, a(16)=203788091,
   a(17)=697249613, a(18)=2385595861, a(19)=8162156545 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for previously computed values. */
    static seq_t *cache   = NULL;   /* array of size cap, index 0..cap-1 */
    static idx_t max_n     = 0;      /* highest index computed so far   */
    static size_t cap      = 0;      /* current allocated size          */

    /* Hard‑coded initial terms (index 0 unused). */
    static const seq_t base[20] = {
        0ULL,                /* 0 – unused */
        8ULL,                /* a(1) */
        9ULL,                /* a(2) */
        26ULL,               /* a(3) */
        81ULL,               /* a(4) */
        267ULL,              /* a(5) */
        938ULL,              /* a(6) */
        3186ULL,             /* a(7) */
        10840ULL,            /* a(8) */
        37115ULL,            /* a(9) */
        127093ULL,           /* a(10) */
        434701ULL,           /* a(11) */
        1487032ULL,          /* a(12) */
        5088052ULL,          /* a(13) */
        17408881ULL,         /* a(14) */
        59562461ULL,         /* a(15) */
        203788091ULL,        /* a(16) */
        697249613ULL,        /* a(17) */
        2385595861ULL,       /* a(18) */
        8162156545ULL        /* a(19) */
    };

    /* Not used by the benchmark: return 0 for indices < 1. */
    if (n < 1) {
        return 0;
    }

    /* Grow cache if necessary to accommodate index n. */
    if ((idx_t)cap < n + 1) {
        size_t new_cap = (size_t)n + 1;  /* indices 0 .. n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out‑of‑memory */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute any missing terms up to index n. */
    if (n > max_n) {
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i <= 19) {
                cache[i] = base[i];
            } else {
                /* Recurrence for n > 19.  All operations are valid for
                   seq_t because only +, -, *, and constants are used.    */
                seq_t val = 2 * cache[i-1]
                          + 5 * cache[i-2]
                          +       cache[i-3]
                          - 11 * cache[i-5]
                          - 24 * cache[i-6]
                          + 5  * cache[i-7]
                          + 17 * cache[i-8]
                          - 5  * cache[i-9]
                          + 6  * cache[i-10]
                          + 14 * cache[i-11]
                          - 5  * cache[i-12]
                          -10  * cache[i-13]
                          + 2  * cache[i-14]
                          + 4  * cache[i-15];
                cache[i] = val;
            }
        }
        max_n = n;
    }

    return cache[n];
}
