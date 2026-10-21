/*
 * A252855  Number of (n+2) X (2+2) 0..2 arrays with every consecutive three
 *          elements in every row, column, diagonal and antidiagonal having
 *          exactly two distinct values, and new values 0 upwards introduced
 *          in row major order.
 * https://oeis.org/A252855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 279, a(2) = 432, a(3) = 738, a(4) = 1296, a(5) = 2502,
 *            a(6) = 4986, a(7) = 9936, a(8) = 20052, a(9) = 40770. For n ≥
 *            10: a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4) - 3*a(n-5) -
 *            4*a(n-6) - 2*a(n-7) + 2*a(n-8) + 2*a(n-9); (All arithmetic is in
 *            the type seq_t.)
 * Range:     u8 none, u16 n <= 9, u32 n <= 25, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/*   seq_t and idx_t are defined by the build environment; we don't
    define them here. */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, but callers should not request it */

    /* static data for memoisation */
    static seq_t *cache = NULL;   /* array of computed terms  (1‑based index -> cache[n-1]) */
    static size_t cap   = 0;      /* allocated size of cache   */
    static size_t num   = 0;      /* number of terms currently stored (largest index) */

    /* initialise with first 9 known terms */
    if (num == 0) {
        size_t init = 32;              /* arbitrary starting capacity */
        cache = (seq_t*)malloc(init * sizeof(seq_t));
        if (cache == NULL) return 0;   /* fallback on out‑of‑memory */
        cap = init;

        cache[0] = (seq_t)279;         /* a(1) */
        cache[1] = (seq_t)432;         /* a(2) */
        cache[2] = (seq_t)738;         /* a(3) */
        cache[3] = (seq_t)1296;        /* a(4) */
        cache[4] = (seq_t)2502;        /* a(5) */
        cache[5] = (seq_t)4986;        /* a(6) */
        cache[6] = (seq_t)9936;        /* a(7) */
        cache[7] = (seq_t)20052;       /* a(8) */
        cache[8] = (seq_t)40770;       /* a(9) */
        num = 9;
    }

    /* compute any terms up to the requested index */
    if ((size_t)n > num) {
        size_t target = (size_t)n;     /* target number of terms */

        /* grow the array if necessary */
        if (target > cap) {
            size_t newcap = cap;
            while (newcap < target) newcap <<= 1;   /* double until enough */
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* out of memory fallback */
            cache = tmp;
            cap   = newcap;
        }

        /* fill terms num … target-1 (0‑based indices) */
        for (size_t k = num; k < target; ++k) {
            seq_t val =
                cache[k-1]                                 /* a(k)   = a(n-1) */
              + cache[k-2]                                 /* a(k-1) = a(n-2) */
              + (seq_t)3 * cache[k-3]                     /* 3*a(k-2)=3*a(n-3) */
              + cache[k-4]                                 /* a(k-3)=a(n-4) */
              - (seq_t)3 * cache[k-5]                     /* 3*a(k-4)=3*a(n-5) */
              - (seq_t)4 * cache[k-6]                     /* 4*a(k-5)=4*a(n-6) */
              - (seq_t)2 * cache[k-7]                     /* 2*a(k-6)=2*a(n-7) */
              + (seq_t)2 * cache[k-8]                     /* 2*a(k-7)=2*a(n-8) */
              + (seq_t)2 * cache[k-9];                    /* 2*a(k-8)=2*a(n-9) */

            cache[k] = val;
        }
        num = target;
    }

    return cache[(size_t)n - 1];
}
