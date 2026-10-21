/*
 * A253321  Number of (n+1)X(3+1) 0..1 arrays with every 2X2 subblock sum
 *          nondecreasing horizontally, vertically and antidiagonally
 *          ne-to-sw.
 * https://oeis.org/A253321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1)+2*a(n-2)-26*a(n-3)+16*a(n-4)+62*a(n-5)
 *             -70*a(n-6)-62*a(n-7)+109*a(n-8)+14*a(n-9)-76*a(n-10)
 *             +16*a(n-11)+20*a(n-12)-8*a(n-13)   for n > 15
 * Initial values: a(1)=121, a(2)=373, a(3)=1217, a(4)=2538, a(5)=6190,
 *                 a(6)=11795, a(7)=25659, a(8)=48085, a(9)=99653,
 *                 a(10)=188562, a(11)=382854, a(12)=736066,
 *                 a(13)=1481812, a(14)=2889153, a(15)=5795537.
 */

#include <stdlib.h>   /* abort, malloc, realloc */
#include <stdint.h>   /* if needed for types */

/* Compute the nth term of OEIS sequence A253321 (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* Initial table for n = 1 … 15.  Values are small enough to fit in any
       unsigned/unsigned‑int compiler type.  The literals are cast to seq_t
       to keep the compiler happy with any seq_t definition. */
    static const seq_t init[15] = {
        (seq_t)121ULL, (seq_t)373ULL, (seq_t)1217ULL, (seq_t)2538ULL,
        (seq_t)6190ULL, (seq_t)11795ULL, (seq_t)25659ULL, (seq_t)48085ULL,
        (seq_t)99653ULL, (seq_t)188562ULL, (seq_t)382854ULL, (seq_t)736066ULL,
        (seq_t)1481812ULL, (seq_t)2889153ULL, (seq_t)5795537ULL
    };

    /* Static cache of previously computed terms (1‑based indices). */
    static seq_t *cache = NULL;
    static idx_t cache_max = 0;     /* size of allocated array (0 … cache_max-1) */
    static idx_t last_computed = 0; /* greatest n for which cache[n] is valid */

    /* First call: allocate cache large enough for the current n or for 15
       values, whichever is bigger. */
    if (cache == NULL) {
        idx_t sz = (n > 15) ? n + 1 : 16;  /* +1 because we ignore index 0 */
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (!cache) abort();
        cache_max = sz;

        /* Initialise the first 15 values. */
        for (idx_t i = 1; i <= 15 && i < cache_max; ++i)
            cache[i] = init[i - 1];
        last_computed = (cache_max <= 15) ? cache_max - 1 : 15;
    }

    /* Ensure the cache can hold up to n.  If not, reallocate. */
    if (cache_max <= n) {
        idx_t new_size = n + 1;            /* extend to index n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_max = new_size;
    }

    /* If we already have a computed value for n, return it immediately. */
    if (n <= last_computed)
        return cache[n];

    /* Compute any missing values using the recurrence. */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        if (k <= 15) {                     /* safety: maybe k < 15 after re‑size */
            cache[k] = init[k - 1];
            continue;
        }

        seq_t pos = 0, neg = 0;
        pos += ((seq_t)4) * cache[k - 1];
        pos += ((seq_t)2) * cache[k - 2];
        neg += ((seq_t)26) * cache[k - 3];
        pos += ((seq_t)16) * cache[k - 4];
        pos += ((seq_t)62) * cache[k - 5];
        neg += ((seq_t)70) * cache[k - 6];
        neg += ((seq_t)62) * cache[k - 7];
        pos += ((seq_t)109) * cache[k - 8];
        pos += ((seq_t)14) * cache[k - 9];
        neg += ((seq_t)76) * cache[k -10];
        pos += ((seq_t)16) * cache[k -11];
        pos += ((seq_t)20) * cache[k -12];
        neg += ((seq_t)8) * cache[k -13];
        cache[k] = pos - neg;
    }

    last_computed = n;
    return cache[n];
}
