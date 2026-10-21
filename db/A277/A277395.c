/*
 * A277395  a(n) = Sum_{k=0..n} binomial(n+1,k+1)*A001003(k).
 * https://oeis.org/A277395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ( (-coeff1)*a(n-1) + coeff2*a(n-2) + (-coeff3)*a(n-3) +
 *            coeff4*a(n-4) ) / (n+1) * where coeff1 = 2*(-5*n+1), coeff2 =
 *            25*n-23, coeff3 = 12*(-2*n+3), coeff4 = 8*(n-2) * (Recurrence
 *            valid for n >= 4). a(0)=1, a(1)=3, a(2)=9, a(3)=33
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cached values of a(n)  */
static size_t  cache_cap = 0;        /* allocated capacity         */
static idx_t   cache_max = (idx_t)-1;/* max index computed so far */

static void
ensure_cap(idx_t n)
{
    if ((size_t)n < cache_cap) return;
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                 /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache with the first four terms */
    if (cache == NULL) {
        ensure_cap(4);                 /* need indices 0..3 at least */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)33;
        cache_max = 3;
    }

    if (n <= cache_max) return cache[n];

    /* compute terms up to n, inclusive */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* coefficients as signed integers */
        idx_t coeff1 =  2 * ((idx_t)(-5) * i + 1);          /* -10*i + 2           */
        idx_t coeff2 =  25 * i - 23;                       /* 25*i - 23           */
        idx_t coeff3 = 12 * ((idx_t)(-2) * i + 3);          /* -24*i + 36          */
        idx_t coeff4 =  8 * (i - 2);                       /* 8*i - 16            */

        /* positive parts (coeff2, coeff4) */
        seq_t pos = (seq_t)coeff2 * cache[i - 2] + (seq_t)coeff4 * cache[i - 4];

        /* negative parts (abs(coeff1), abs(coeff3)) */
        seq_t neg = (seq_t)(-coeff1) * cache[i - 1] + (seq_t)(-coeff3) * cache[i - 3];

        /* difference: neg - pos (always non‑negative) */
        seq_t diff = neg - pos;

        /* a(i) = diff / (i+1)  */
        seq_t val = diff / (seq_t)(i + 1);

        ensure_cap(i);
        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
