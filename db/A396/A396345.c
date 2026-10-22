/*
 * A396345  Expansion of x *
 *          (3+3*x-8*x^2-3*x^3+2*x^4-3*x^5+4*x^6+x^7-x^8)/((1-x)^2 * (1+x) *
 *          (1-3*x^2-x^4+x^6)).
 * https://oeis.org/A396345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 4*a(n-2) - 4*a(n-3) - 2*a(n-4) + 2*a(n-5) -
 *            2*a(n-6) + 2*a(n-7) + a(n-8) - a(n-9) initial values: a(1)=3,
 *            a(2)=6, a(3)=10, a(4)=19, a(5)=31, a(6)=58, a(7)=96, a(8)=181,
 *            a(9)=303
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cap   = 0;
static idx_t  max_computed = 0;

static void ensure_capacity(size_t needed)
{
    if (cap <= needed) {
        size_t newcap = cap ? cap * 2 : 16;
        while (newcap <= needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }
}

static void init_base()
{
    if (cap < 10) {
        ensure_capacity(10);
        cache[1] =  3;
        cache[2] =  6;
        cache[3] = 10;
        cache[4] = 19;
        cache[5] = 31;
        cache[6] = 58;
        cache[7] = 96;
        cache[8] = 181;
        cache[9] = 303;
        max_computed = 9;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* out of range, not used */
    init_base();
    ensure_capacity((size_t)n);

    if (n <= max_computed) return cache[n];

    for (size_t i = (size_t)max_computed + 1; i <= (size_t)n; ++i) {
        seq_t pos = cache[i-1];               /* + a(n-1)               */
        pos += 4 * cache[i-2];               /* + 4*a(n-2)              */
        pos += 2 * cache[i-5];               /* + 2*a(n-5)              */
        pos += 2 * cache[i-7];               /* + 2*a(n-7)              */
        pos +=       cache[i-8];             /* + a(n-8)                */

        seq_t neg = 4 * cache[i-3];           /* - 4*a(n-3)              */
        neg += 2 * cache[i-4];               /* - 2*a(n-4)              */
        neg += 2 * cache[i-6];               /* - 2*a(n-6)              */
        neg +=       cache[i-9];             /* - a(n-9)                */

        cache[i] = pos - neg;                /* combine positive and negative parts */
    }

    max_computed = n;
    return cache[n];
}
