/*
 * A304216  Number of n X 3 0..1 arrays with every element unequal to 0, 1, 3
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 39, u64 n <= 80, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-3) + 2*a(n-4)  for n > 5
   a(1) = 3,  a(2) = 7,  a(3) = 10, a(4) = 17, a(5) = 32  */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out‑of‑range guard */

    /* static cache: cache[0] is unused, cache[1]..cache[last] are set  */
    static seq_t *cache   = NULL;
    static size_t  cap    = 0;            /* number of allocated elements  */
    static idx_t   last   = 0;            /* largest index already computed */

    /* initial allocation                                 */
    if (cache == NULL) {
        cap   = 64;                       /* initial size (>= 6)            */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed              */
    }

    /* grow cache if necessary (ensure room for indices 1..n) */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1 + 32;  /* some slack */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;     /* allocation failed              */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1)       cache[i] = (seq_t)3;
        else if (i == 2)  cache[i] = (seq_t)7;
        else if (i == 3)  cache[i] = (seq_t)10;
        else if (i == 4)  cache[i] = (seq_t)17;
        else if (i == 5)  cache[i] = (seq_t)32;
        else {
            seq_t v1 = cache[i-1];
            seq_t v3 = cache[i-3];
            seq_t v4 = cache[i-4];
            cache[i] = v1 + v3 + (v4 << 1);  /* 2*v4 */
        }
    }
    last = n;
    return cache[n];
}
