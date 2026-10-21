/*
 * A257934  Expansion of 1/(1-x-x^2-x^3-x^4+x^5+x^6+x^7).
 * https://oeis.org/A257934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   / a(0)=1, a(1)=1, a(2)=2, a(3)=4, a(4)=8, a(5)=14, a(6)=26
 *            a(n)=a(n-1)+a(n-2)+a(n-3)+a(n-4)-a(n-5)-a(n-6)-a(n-7) (n>=7)
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, realloc, abort */

/* External types supplied by the build system */
extern seq_t a(idx_t n);

/* Implements the sequence with memoisation.  The cache is allocated
   dynamically and grows as needed.  The code is valid under both
   C11 and C++17. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* dynamic array of the sequence */
    static size_t cap     = 0;      /* allocated capacity */
    static size_t len     = 0;      /* number of computed entries   */

    /* Allocate/expand the cache if required */
    if (cap < (size_t)n + 1) {
        size_t newcap = cap ? cap * 2 : 64;
        if (newcap < (size_t)n + 1) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory – terminate */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise base cases once */
    if (len == 0) {
        cache[0] = 1;
        if (n == 0) return cache[0];
        cache[1] = 1;
        if (n == 1) return cache[1];
        cache[2] = 2;
        if (n == 2) return cache[2];
        cache[3] = 4;
        if (n == 3) return cache[3];
        cache[4] = 8;
        if (n == 4) return cache[4];
        cache[5] = 14;
        if (n == 5) return cache[5];
        cache[6] = 26;
        if (n == 6) return cache[6];
        len = 7;                       /* we have values for indices 0..6   */
    }

    /* Compute any missing values up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t val = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4]
                    - cache[i-5] - cache[i-6] - cache[i-7];
        cache[i] = val;
    }
    len = (size_t)n + 1;
    return cache[n];
}
