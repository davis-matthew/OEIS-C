/*
 * A301321  Number of nX6 0..1 arrays with every element equal to 0, 1, 2, 5
 *          or 6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A301321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 23, u64 n <= 51, u128 n <= 108,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (OEIS formula):
   a(n) = 5*a(n-1) - 8*a(n-2) + 4*a(n-3) + 2*a(n-7) - 2*a(n-8)
          - 3*a(n-9) + 5*a(n-10) - a(n-11) - a(n-12) - a(n-14) + a(n-15)
   (n > 19)
   Initial values:
   a(1)=32, a(2)=383, a(3)=735, a(4)=1709, a(5)=3649,
   a(6)=7600, a(7)=15802, a(8)=33150, a(9)=70311, a(10)=150532,
   a(11)=324443, a(12)=702447, a(13)=1525146, a(14)=3317041,
   a(15)=7221520, a(16)=15731216, a(17)=34280366, a(18)=74716439,
   a(19)=162869203  */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial 19 terms – needed to start the recurrence. */
    static const seq_t seed[19] = {
        (seq_t)32,           (seq_t)383,          (seq_t)735,
        (seq_t)1709,         (seq_t)3649,         (seq_t)7600,
        (seq_t)15802,        (seq_t)33150,        (seq_t)70311,
        (seq_t)150532,       (seq_t)324443,       (seq_t)702447,
        (seq_t)1525146,      (seq_t)3317041,      (seq_t)7221520,
        (seq_t)15731216,     (seq_t)34280366,     (seq_t)74716439,
        (seq_t)162869203
    };

    /* For n <= 19, simply return the known value. */
    if (n <= 19) {
        return seed[n - 1];
    }

    /* Static cache to memoise computed values. */
    static seq_t *cache = NULL;          /* array of values, indexed by 1 … */
    static idx_t  last_computed = 0;     /* highest n we have computed */
    static size_t cache_cap = 0;         /* current capacity of cache      */

    /* Initialise cache on first call or enlarge if needed. */
    if (cache == NULL || n > cache_cap) {
        /* Determine new capacity: at least n, but cap at least 20. */
        size_t new_cap = (size_t) n;
        if (new_cap < 20) new_cap = 20;

        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – return zero. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;

        /* If this is the first allocation, load the seed values. */
        if (last_computed == 0) {
            for (idx_t i = 1; i <= 19 && i <= cache_cap; ++i) {
                cache[i] = seed[i - 1];
            }
            last_computed = 19;
        }
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* Compute using the recurrence formula. */
        /* Use explicit casts to seq_t for integer constants to avoid
           ambiguous overloads in the arbitrary‑precision case. */
        seq_t pos = (seq_t)5 * cache[i - 1]
                  + (seq_t)4 * cache[i - 3]
                  + (seq_t)2 * cache[i - 7]
                  + (seq_t)5 * cache[i - 10]
                  + (seq_t)1 * cache[i - 15];

        seq_t neg = (seq_t)8 * cache[i - 2]
                  + (seq_t)2 * cache[i - 8]
                  + (seq_t)3 * cache[i - 9]
                  + (seq_t)1 * cache[i - 11]
                  + (seq_t)1 * cache[i - 12]
                  + (seq_t)1 * cache[i - 14];

        cache[i] = pos - neg;
    }
    last_computed = n;

    return cache[n];
}
