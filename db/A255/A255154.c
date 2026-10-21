/*
 * A255154  Number of (n+2)X(3+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 1 and no antidiagonal sum 1 and no row sum 0 or 1 and no
 *          column sum 0 or 1.
 * https://oeis.org/A255154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A255154
    a(n) = a(n-1)+a(n-2)+13*a(n-3)+31*a(n-4)+44*a(n-5)+33*a(n-6)
           -50*a(n-7)-84*a(n-8)+a(n-9)+76*a(n-10)
           -86*a(n-11)-274*a(n-12)-149*a(n-13)-14*a(n-14)
           +12*a(n-15)-2*a(n-16)-4*a(n-17)+4*a(n-18)
    with a(1)=141, a(2)=459, a(3)=1637, a(4)=5615, a(5)=19437,
    a(6)=67465, a(7)=232685, a(8)=804189, a(9)=2784175,
    a(10)=9625969, a(11)=33269463, a(12)=115046055,
    a(13)=397861797, a(14)=1375713437, a(15)=4756735709,
    a(16)=16447734043, a(17)=56873715759, a(18)=196658964777,
    a(19)=680003968725, a(20)=2351308608855, a(21)=8130361322703,
    a(22)=28113190609661.
*/

#include <stdlib.h>
#include <stddef.h>

/* Repeated coefficients for the recurrence, coeff[k-1] corresponds
   to a(n-k).  The sequence requires 18 coefficients. */
static const int coeff[18] = {
    1,   1,  13,  31,  44,  33,   -50,  -84,
    1,   76, -86, -274, -149, -14,   12,   -2,
    -4,    4
};

/* The first 22 values of the sequence, indexed from 1. */
static const seq_t init_val[22] = {
    (seq_t)141,         (seq_t)459,        (seq_t)1637,
    (seq_t)5615,        (seq_t)19437,      (seq_t)67465,
    (seq_t)232685,      (seq_t)804189,     (seq_t)2784175,
    (seq_t)9625969,     (seq_t)33269463,   (seq_t)115046055,
    (seq_t)397861797,   (seq_t)1375713437, (seq_t)4756735709,
    (seq_t)16447734043, (seq_t)56873715759,(seq_t)196658964777,
    (seq_t)680003968725,(seq_t)2351308608855,(seq_t)8130361322703,
    (seq_t)28113190609661
};

/* Cached values of a(n).  cache[n] contains a(n) for n>=1.
   cache[0] is unused. */
static seq_t *cache          = NULL;
static size_t cache_cap      = 0;   /* allocated array size */
static size_t cache_next_idx = 1;   /* next index to compute */

/* Ensure the cache contains all a(n) up to n. */
static void
ensure_cache(idx_t n)
{
    /* initialize cache */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) abort();

        /* copy the first 22 known terms */
        for (size_t i = 1; i <= 22; ++i)
            cache[i] = init_val[i - 1];

        /* we have terms up to 22 ready */
        cache_next_idx = 23;   /* next index to compute */
    }

    /* grow the array if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute any missing terms */
    for (size_t i = cache_next_idx; i <= (size_t)n; ++i) {
        seq_t pos = 0, neg = 0;
        /* recurrence uses a(n-1) ... a(n-18) */
        for (int k = 1; k <= 18; ++k) {
            size_t idx = i - k;
            seq_t term = cache[idx];
            int c = coeff[k - 1];
            if (c > 0)
                pos += term * (seq_t)c;
            else
                neg += term * (seq_t)(-c);
        }
        cache[i] = pos - neg;
    }
    cache_next_idx = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* a(0) is undefined */
    if (n <= 22) return init_val[n - 1];    /* first 22 terms */
    ensure_cache(n);                        /* compute as needed */
    return cache[(size_t)n];
}
