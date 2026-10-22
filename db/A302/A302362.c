/*
 * A302362  Number of nX3 0..1 arrays with every element equal to 0, 2, 3 or 4
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A302362
   Recurrence:  a(n) = 4*a(n-1) -3*a(n-2) +9*a(n-3) -9*a(n-4) -11*a(n-5) +a(n-6)
                 -18*a(n-7) +16*a(n-8) -12*a(n-9) -4*a(n-10) +4*a(n-11) +8*a(n-12)
   Initial values (n = 1 … 12):
       1, 12, 31, 109, 397, 1430, 5110, 18395, 66203, 238103, 856413, 3081033
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* 1‑based index, cache[0] unused */
    static size_t cap   = 0;         /* current allocation size */
    static size_t comp  = 0;         /* highest computed index */

    /* Ensure space for n */
    if (n + 1 > cap) {
        size_t new_cap = (cap == 0) ? 16 : cap << 1;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* memory failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise first 12 terms if not yet in cache */
    if (comp < 12) {
        /* 13 entries: index 0 is unused, 1‑12 hold the initial values */
        static const seq_t init[13] = {
            0ULL,            /* cache[0] unused */
            1ULL,            /* a(1)  */
            12ULL,           /* a(2)  */
            31ULL,           /* a(3)  */
            109ULL,          /* a(4)  */
            397ULL,          /* a(5)  */
            1430ULL,         /* a(6)  */
            5110ULL,         /* a(7)  */
            18395ULL,        /* a(8)  */
            66203ULL,        /* a(9)  */
            238103ULL,       /* a(10) */
            856413ULL,       /* a(11) */
            3081033ULL       /* a(12) */
        };
        size_t fill_to = (n < 12) ? n : 12;
        for (size_t i = 1; i <= fill_to; ++i) {
            cache[i] = init[i];
        }
        comp = fill_to;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = comp + 1; i <= n; ++i) {
        seq_t pos = 4 * cache[i-1]      /* 4*a(i-1)   */
                  + 9 * cache[i-3]      /* 9*a(i-3)   */
                  + cache[i-6]          /* 1*a(i-6)   */
                  + 16 * cache[i-8]     /* 16*a(i-8)  */
                  + 4 * cache[i-11]     /* 4*a(i-11)  */
                  + 8 * cache[i-12];    /* 8*a(i-12)  */

        seq_t neg = 3 * cache[i-2]      /* 3*a(i-2)   */
                  + 9 * cache[i-4]      /* 9*a(i-4)   */
                  + 11 * cache[i-5]     /* 11*a(i-5) */
                  + 18 * cache[i-7]     /* 18*a(i-7) */
                  + 12 * cache[i-9]     /* 12*a(i-9) */
                  + 4 * cache[i-10];    /* 4*a(i-10) */

        cache[i] = pos - neg;   /* final recurrence value */
    }
    comp = n;  /* update max computed */

    return cache[n];
}
