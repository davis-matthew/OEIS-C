/*
 * A333602  Number of directed Hamiltonian walks from NW to SW corners of a 6
 *          X n grid.
 * https://oeis.org/A333602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(1)=1, a(2)=1, a(3)=16, a(4)=47, a(5)=397, a(6)=1770, a(7)=11658,
   a(8)=59946, a(9)=359962, a(10)=1958968, a(11)=11341696, a(12)=63142224,
   a(13)=360314940, a(14)=2024278172;
   for n>14: a(n)=5*a(n-1)+14*a(n-2)-63*a(n-3)+12*a(n-4)+90*a(n-5)
             -35*a(n-6)-66*a(n-7)+118*a(n-8)-8*a(n-9)-82*a(n-10)
             +42*a(n-11)+28*a(n-12)-4*a(n-13)+2*a(n-14);
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Cache for already computed values, indexed by n */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len  = 0;   /* number of valid entries; cache_len-1 is max n cached */

    /* Ensure pool is large enough */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* First bootstrap: create a placeholder for index 0 */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    /* If the requested value is already known, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Base values for 1..14 (must be present to start the recurrence) */
    static const seq_t init[15] = {
        (seq_t)0,          /* unused index 0 – just aligns indices */
        (seq_t)1,          /* a(1) */
        (seq_t)1,          /* a(2) */
        (seq_t)16,         /* a(3) */
        (seq_t)47,         /* a(4) */
        (seq_t)397,        /* a(5) */
        (seq_t)1770,       /* a(6) */
        (seq_t)11658,      /* a(7) */
        (seq_t)59946,      /* a(8) */
        (seq_t)359962,     /* a(9) */
        (seq_t)1958968,    /* a(10) */
        (seq_t)11341696,   /* a(11) */
        (seq_t)63142224,   /* a(12) */
        (seq_t)360314940,  /* a(13) */
        (seq_t)2024278172  /* a(14) */
    };

    /* Recurrence coefficients */
    static const int coeff[14] = {
        5, 14, -63, 12, 90, -35, -66, 118, -8, -82, 42, 28, -4, 2
    };

    /* Compute values up to n if needed */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i <= 14) {
            cache[(size_t)i] = init[(size_t)i];
            continue;
        }

        seq_t pos = 0, neg = 0;

        /* Apply recurrence: separate positive and negative parts to avoid signed casts on unsigned types */
        for (int j = 0; j < 14; ++j) {
            seq_t term = cache[(size_t)(i - 1 - j)];
            int c = coeff[j];
            if (c >= 0)
                pos += term * (seq_t)c;
            else
                neg += term * (seq_t)(-c);
        }

        cache[(size_t)i] = pos - neg;
    }

    cache_len = (size_t)(n + 1);
    return cache[(size_t)n];
}
