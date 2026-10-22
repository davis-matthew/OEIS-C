/*
 * A309689  Number of even parts appearing among the second largest parts of
 *          the partitions of n into 3 parts.
 * https://oeis.org/A309689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..⌊n/3⌋} Sum_{i=j..⌊(n-j)/2⌋} ((i-1) mod 2)
 *            Recurrence:
 *            a(n)=2*a(n-1)-2*a(n-2)+2*a(n-3)-a(n-4)+a(n-6)-2*a(n-7)+2*a(n-8)-2*a(n-9)+a(n-10)
 *            for n>9 Initial terms:
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=1,a(6)=2,a(7)=2,a(8)=2,a(9)=3
 * Range:     u8 n <= 78, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static bool  ready = false;
    size_t need = (size_t)n + 1;

    /* Allocate or grow cache */
    if (cache == NULL) {
        cap = 64;                         /* arbitrary initial size */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    if (need > cap) {
        size_t newcap = cap;
        while (newcap < need) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Initialise first ten terms once */
    if (!ready) {
        cache[0] = 0; cache[1] = 0; cache[2] = 0; cache[3] = 0; cache[4] = 0;
        cache[5] = 1; cache[6] = 2; cache[7] = 2; cache[8] = 2; cache[9] = 3;
        ready = true;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = 10; i <= n; ++i) {
        int64_t v = 0;
        v +=  2LL * (int64_t)cache[i-1];
        v -=  2LL * (int64_t)cache[i-2];
        v +=  2LL * (int64_t)cache[i-3];
        v -=   1LL * (int64_t)cache[i-4];
        v +=   1LL * (int64_t)cache[i-6];
        v -=  2LL * (int64_t)cache[i-7];
        v +=  2LL * (int64_t)cache[i-8];
        v -=  2LL * (int64_t)cache[i-9];
        v +=   1LL * (int64_t)cache[i-10];
        cache[i] = (seq_t)v;
    }

    return cache[(size_t)n];
}
