/*
 * A291105  Number of maximal irredundant sets in the n-web graph.
 * https://oeis.org/A291105
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            2*a(n-1)+a(n-2)+4*a(n-3)-2*a(n-4)+4*a(n-5)+3*a(n-6)-26*a(n-7)-14*a(n-8)-48*a(n-9)+65*a(n-10)+23*a(n-11)-22*a(n-12)+10*a(n-13)-76*a(n-14)+54*a(n-15)+2*a(n-16)+21*a(n-17)
 *            for n>=20 with initial values:
 *            a(3)=26,a(4)=58,a(5)=182,a(6)=540,a(7)=1290,a(8)=3690,a(9)=10160,a(10)=28786,a(11)=80841,a...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset of first term */
    const idx_t OFFSET = 3;

    /* Initial 17 terms a(3)..a(19) */
    static const seq_t init_vals[17] = {
        (seq_t)26ULL,
        (seq_t)58ULL,
        (seq_t)182ULL,
        (seq_t)540ULL,
        (seq_t)1290ULL,
        (seq_t)3690ULL,
        (seq_t)10160ULL,
        (seq_t)28786ULL,
        (seq_t)80841ULL,
        (seq_t)223888ULL,
        (seq_t)624613ULL,
        (seq_t)1743972ULL,
        (seq_t)4865441ULL,
        (seq_t)13595466ULL,
        (seq_t)37944869ULL,
        (seq_t)105893100ULL,
        (seq_t)295608956ULL
    };
    const size_t INIT_LEN = 17;   /* number of precomputed terms */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0; /* allocated capacity */
    static size_t len   = 0; /* number of terms stored */
    const size_t INITIAL_CAP = 64; /* first allocation size */

    /* initialise cache on first use */
    if (!cache) {
        cap = INITIAL_CAP;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – fall back to 0 */
        for (size_t i = 0; i < INIT_LEN; ++i)
            cache[i] = init_vals[i];
        len = INIT_LEN;
    }

    /* no meaning for n < 3, return 0 */
    if (n < OFFSET)
        return (seq_t)0;

    /* index of desired term in cache is n - 3 */
    size_t target_index = (size_t)n - OFFSET;
    if (target_index < len)
        return cache[target_index];

    /* ensure enough capacity */
    while (len <= target_index) {
        if (cap < len + 1) {
            size_t new_cap = (cap == 0) ? INITIAL_CAP : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0; /* allocation failure */
            cache = tmp;
            cap = new_cap;
        }

        size_t i = 3 + len; /* current n to compute */
        /* compute positive and negative parts separately to avoid
           underflow in unsigned arithmetic */
        seq_t pos = 0, neg = 0;

        /* positive contributions */
        pos += (seq_t)2 * cache[i - 4]; /* 2*a(n-1) */
        pos +=         cache[i - 5];    /*  a(n-2) */
        pos += (seq_t)4 * cache[i - 6]; /* 4*a(n-3) */
        pos += (seq_t)4 * cache[i - 8]; /* 4*a(n-5) */
        pos += (seq_t)3 * cache[i - 9]; /* 3*a(n-6) */
        pos += (seq_t)65 * cache[i - 13];/* 65*a(n-10) */
        pos += (seq_t)23 * cache[i - 14];/* 23*a(n-11) */
        pos += (seq_t)10 * cache[i - 16];/* 10*a(n-13) */
        pos += (seq_t)54 * cache[i - 18];/* 54*a(n-15) */
        pos += (seq_t)2 * cache[i - 19];/*  2*a(n-16) */
        pos += (seq_t)21 * cache[i - 20];/* 21*a(n-17) */

        /* negative contributions */
        neg += (seq_t)2 * cache[i - 7];  /* 2*a(n-4) */
        neg += (seq_t)26 * cache[i -10]; /* 26*a(n-7) */
        neg += (seq_t)14 * cache[i -11]; /* 14*a(n-8) */
        neg += (seq_t)48 * cache[i -12]; /* 48*a(n-9) */
        neg += (seq_t)22 * cache[i -15]; /* 22*a(n-12) */
        neg += (seq_t)76 * cache[i -17]; /* 76*a(n-14) */

        seq_t val = pos - neg;
        cache[len++] = val;
    }

    return cache[target_index];
}
