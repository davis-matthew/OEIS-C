/*
 * A318541  Number of nX4 0..1 arrays with every element unequal to 0, 1, 4 or
 *          6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A318541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A318541
    Empirical recurrence (for n>14):
      a(n) = a(n-1) +7*a(n-3) +3*a(n-4) +10*a(n-5)
           -a(n-6) +4*a(n-7) -6*a(n-8) -a(n-9)
           -a(n-10) -a(n-12) +a(n-13)
    Base values:
      a(1)=5, a(2)=5, a(3)=13, a(4)=31, a(5)=78, a(6)=207, a(7)=516,
      a(8)=1288, a(9)=3270, a(10)=8271, a(11)=20866, a(12)=52679,
      a(13)=133064, a(14)=336062
    The function computes a(n) for any n>=1, memoising results for
    efficiency.  All arithmetic is performed in seq_t, which may be a
    native unsigned integer or an arbitrary‑precision integer type.
    The implementation avoids signed arithmetic in the recurrence to
    prevent underflow when using unsigned seq_t types.    */

#include <stdlib.h>
#include <stddef.h>

/* base values 0..14, index 0 unused */
static const seq_t BASE[15] = {
    0,
    (seq_t)5, (seq_t)5, (seq_t)13, (seq_t)31, (seq_t)78,
    (seq_t)207, (seq_t)516, (seq_t)1288, (seq_t)3270,
    (seq_t)8271, (seq_t)20866, (seq_t)52679, (seq_t)133064,
    (seq_t)336062
};

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;   /* largest n already computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 15;                     /* indices 0..14 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = 0;
        for (idx_t i = 1; i <= 14; ++i)
            cache[i] = BASE[i];
        computed = 14;
    }

    /* grow cache if necessary */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = 0;
        cache_size = new_size;
    }

    if (n <= computed)
        return cache[n];

    /* compute values from computed+1 up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 14) {          /* base values already set */
            cache[i] = BASE[i];
            continue;
        }
        /* compute using unsigned arithmetic, grouping positives and negatives */
        seq_t pos = cache[i-1];
        pos += cache[i-3] * (seq_t)7;
        pos += cache[i-4] * (seq_t)3;
        pos += cache[i-5] * (seq_t)10;
        pos += cache[i-7] * (seq_t)4;
        pos += cache[i-13];

        seq_t neg = cache[i-6];
        neg += cache[i-8] * (seq_t)6;
        neg += cache[i-9];
        neg += cache[i-10];
        neg += cache[i-12];

        cache[i] = pos - neg;   /* assumed pos >= neg for this sequence */
    }

    computed = n;
    return cache[n];
}
