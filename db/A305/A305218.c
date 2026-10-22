/*
 * A305218  Number of nX3 0..1 arrays with every element unequal to 2, 3, 5, 6
 *          or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence (empirical) for n > 18 a(n) = 2*a(n-1) + 5*a(n-2)
 *            -12*a(n-4) -15*a(n-5) + 3*a(n-6) +30*a(n-7) +14*a(n-8)
 *            -40*a(n-9)-44*a(n-10)+23*a(n-11)+66*a(n-12)
 *            +22*a(n-13)-15*a(n-14)- 6*a(n-15)
 *            (a(1)=0,a(2)=4,a(3)=8,a(4)=15,a(5)=31,a(6)=92,a(7)=264,a(8)=784,
 *            a(9)=2378,a(10)=7342,a(11)=22596,a(12)...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[18] = {
    (seq_t)0,  (seq_t)4,  (seq_t)8,  (seq_t)15, (seq_t)31,
    (seq_t)92, (seq_t)264, (seq_t)784, (seq_t)2378, (seq_t)7342,
    (seq_t)22596, (seq_t)69617, (seq_t)214983, (seq_t)664217,
    (seq_t)2052307, (seq_t)6342935, (seq_t)19606074, (seq_t)60603932
};

seq_t a(idx_t n)
{
    /* handle the first 18 terms directly */
    if (n <= 18)
        return init_terms[(size_t)n - 1];

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache on first non-trivial call */
    if (cache_len == 0) {
        cache_len = 18;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* out of memory; the caller should handle */
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init_terms[i];
    }

    /* extend cache up to requested index */
    while (cache_len < (size_t)n) {
        size_t i = cache_len + 1;        /* current n being computed (1‑based) */

        /* indices into cache (0‑based) */
        seq_t a1 = cache[i - 2];  /* a(n-1) */
        seq_t a2 = cache[i - 3];  /* a(n-2) */
        seq_t a4 = cache[i - 5];  /* a(n-4) */
        seq_t a5 = cache[i - 6];  /* a(n-5) */
        seq_t a6 = cache[i - 7];  /* a(n-6) */
        seq_t a7 = cache[i - 8];  /* a(n-7) */
        seq_t a8 = cache[i - 9];  /* a(n-8) */
        seq_t a9 = cache[i -10];  /* a(n-9) */
        seq_t a10= cache[i -11];  /* a(n-10) */
        seq_t a11= cache[i -12];  /* a(n-11) */
        seq_t a12= cache[i -13];  /* a(n-12) */
        seq_t a13= cache[i -14];  /* a(n-13) */
        seq_t a14= cache[i -15];  /* a(n-14) */
        seq_t a15= cache[i -16];  /* a(n-15) */

        seq_t next = 0;
        next += (seq_t)2 * a1;
        next += (seq_t)5 * a2;
        next -= (seq_t)12 * a4;
        next -= (seq_t)15 * a5;
        next += (seq_t)3 * a6;
        next += (seq_t)30 * a7;
        next += (seq_t)14 * a8;
        next -= (seq_t)40 * a9;
        next -= (seq_t)44 * a10;
        next += (seq_t)23 * a11;
        next += (seq_t)66 * a12;
        next += (seq_t)22 * a13;
        next -= (seq_t)15 * a14;
        next -= (seq_t)6 * a15;

        /* extend array to hold the new term */
        size_t new_len = cache_len + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* memory failure */
        cache = tmp;
        cache[cache_len] = next;
        cache_len = new_len;
    }

    return cache[(size_t)n - 1];
}
