/*
 * A247536  Number of length 4+3 0..n arrays with some disjoint pairs in every
 *          consecutive four terms having the same sum.
 * https://oeis.org/A247536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 349, u64 n <= 349,
 *            u128 n <= 349, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A247536.
   Recurrence (valid for n ≥ 25):
   a(n) = -a(n-1) -a(n-2) +a(n-4) +2*a(n-5) +3*a(n-6) +3*a(n-7)
          +2*a(n-8) -2*a(n-10) -4*a(n-11) -4*a(n-12) -4*a(n-13) -2*a(n-14)
          +2*a(n-16) +3*a(n-17) +3*a(n-18) +2*a(n-19) +a(n-20)
          -a(n-22) -a(n-23) -a(n-24)
   Initial values (offset 1):
   a(1)=8, a(2)=81, a(3)=364, a(4)=1007, a(5)=2164,
   a(6)=3997, a(7)=6584, a(8)=10219, a(9)=14852, a(10)=20847,
   a(11)=28108, a(12)=37095, a(13)=47564, a(14)=60087, a(15)=74428,
   a(16)=91101, a(17)=109760, a(18)=131243, a(19)=154956,
   a(20)=181677, a(21)=211024, a(22)=243709, a(23)=279136,
   a(24)=318445 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache: indices start at 0 (unused), indices 1..24 are
       the known initial values. cache_len holds the highest computed
       index.  Values are stored in a[0..cache_len]. */

    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cache_len = 0;  /* largest index computed */
    static size_t cache_cap = 0;  /* allocated capacity (+1 for index 0) */

    /* Initial values (index 0 unused). */
    static const seq_t init[25] = {
        0ULL,          /* dummy for index 0 */
        8ULL,          /* a(1) */
        81ULL,         /* a(2) */
        364ULL,        /* a(3) */
        1007ULL,       /* a(4) */
        2164ULL,       /* a(5) */
        3997ULL,       /* a(6) */
        6584ULL,       /* a(7) */
        10219ULL,      /* a(8) */
        14852ULL,      /* a(9) */
        20847ULL,      /* a(10) */
        28108ULL,      /* a(11) */
        37095ULL,      /* a(12) */
        47564ULL,      /* a(13) */
        60087ULL,      /* a(14) */
        74428ULL,      /* a(15) */
        91101ULL,      /* a(16) */
        109760ULL,     /* a(17) */
        131243ULL,     /* a(18) */
        154956ULL,     /* a(19) */
        181677ULL,     /* a(20) */
        211024ULL,     /* a(21) */
        243709ULL,     /* a(22) */
        279136ULL,     /* a(23) */
        318445ULL      /* a(24) */
    };

    /* In the OEIS offset is 1: a(1) is the first term. */
    if (n == 0) return (seq_t)0;            /* undefined for n<1, return 0 */
    if (n <= 24U) return init[(size_t)n];

    /* Ensure the cache is allocated and contains all initial values. */
    if (cache_len == 0) {
        /* Allocate a reasonable starting size. */
        cache_cap = (size_t)n > 256 ? (size_t)n : 256;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) abort();                /* out of memory */
        cache[0] = (seq_t)0;
        for (size_t i = 1; i <= 24; ++i) cache[i] = init[i];
        cache_len = 24;
    }
    /* Grow the cache if necessary. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (size_t)n > (cache_cap * 2) ? (size_t)n : (cache_cap * 2);
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        /* Compute positive part first. */
        seq_t pos = cache[i - 4]
                  + (seq_t)2 * cache[i - 5]
                  + (seq_t)3 * cache[i - 6]
                  + (seq_t)3 * cache[i - 7]
                  + (seq_t)2 * cache[i - 8]
                  + (seq_t)2 * cache[i - 16]
                  + (seq_t)3 * cache[i - 17]
                  + (seq_t)3 * cache[i - 18]
                  + (seq_t)2 * cache[i - 19]
                  + cache[i - 20];

        /* Compute negative part. */
        seq_t neg = cache[i - 1]
                  + cache[i - 2]
                  + (seq_t)2 * cache[i - 10]
                  + (seq_t)4 * cache[i - 11]
                  + (seq_t)4 * cache[i - 12]
                  + (seq_t)4 * cache[i - 13]
                  + (seq_t)2 * cache[i - 14]
                  + cache[i - 22]
                  + cache[i - 23]
                  + cache[i - 24];

        /* The recurrence guarantees pos ≥ neg. */
        cache[i] = pos - neg;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n];
}
