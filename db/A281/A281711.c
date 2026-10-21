/*
 * A281711  Number of nX4 0..1 arrays with no element unequal to a strict
 *          majority of its horizontal, diagonal and antidiagonal neighbors
 *          and with new values introduced in order 0 sequentially upwards.
 * https://oeis.org/A281711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -2*a(n-2) -4*a(n-3) -2*a(n-4) +a(n-5) +7*a(n-6)
 *            +a(n-7) -10*a(n-8) -a(n-9) +27*a(n-10) -8*a(n-11) -18*a(n-12)
 *            +8*a(n-13) +a(n-14) -3*a(n-15) for n > 16 Initial values:
 *            a(1)=2, a(2)=14, a(3)=38, a(4)=97, a(5)=245, a(6)=631,
 *            a(7)=1625, a(8)=4234, a(9)=11017, a(10)=28652, a(11)=74521...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* pre‑computed values for indices 1…16 */
static const seq_t init_vals[16] = {
    (seq_t)2,      /* a(1) */
    (seq_t)14,     /* a(2) */
    (seq_t)38,     /* a(3) */
    (seq_t)97,     /* a(4) */
    (seq_t)245,    /* a(5) */
    (seq_t)631,    /* a(6) */
    (seq_t)1625,   /* a(7) */
    (seq_t)4234,   /* a(8) */
    (seq_t)11017,  /* a(9) */
    (seq_t)28652,  /* a(10) */
    (seq_t)74521,  /* a(11) */
    (seq_t)193836, /* a(12) */
    (seq_t)504195, /* a(13) */
    (seq_t)1311543,/* a(14) */
    (seq_t)3411786,/* a(15) */
    (seq_t)8875377 /* a(16) */
};

/* compute a(n) */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;        /* number of terms already computed */

    /* initialise cache on first call */
    if (cache_sz == 0) {
        cache_sz = 16;
        cache = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = init_vals[i];
    }

    /* return cached value if available */
    if (n <= cache_sz)
        return cache[n - 1];

    /* resize cache to hold up to n terms */
    if (n > cache_sz) {
        cache_sz = n;
        cache = (seq_t *)realloc(cache, cache_sz * sizeof(seq_t));
    }

    /* compute successive terms from a(17) up to a(n) */
    for (idx_t i = 16; i < n; ++i) {
        /* terms are indexed in cache as a(1) -> cache[0] */
        seq_t pos = cache[i - 1] * (seq_t)4                  /* +4*a(n-1) */
                 + cache[i - 5]                               /* +a(n-5)  */
                 + cache[i - 6] * (seq_t)7                  /* +7*a(n-6) */
                 + cache[i - 7]                               /* +a(n-7)  */
                 + cache[i -10] * (seq_t)27                 /* +27*a(n-10) */
                 + cache[i -13] * (seq_t)8                  /* +8*a(n-13) */
                 + cache[i -14];                            /* +a(n-14) */

        seq_t neg = cache[i - 2] * (seq_t)2                  /* -2*a(n-2) */
                 + cache[i - 3] * (seq_t)4                  /* -4*a(n-3) */
                 + cache[i - 4] * (seq_t)2                  /* -2*a(n-4) */
                 + cache[i - 8] * (seq_t)10                 /* -10*a(n-8) */
                 + cache[i - 9]                              /* -a(n-9) */
                 + cache[i -11] * (seq_t)8                  /* -8*a(n-11) */
                 + cache[i -12] * (seq_t)18                 /* -18*a(n-12) */
                 + cache[i -15] * (seq_t)3;                 /* -3*a(n-15) */

        /* guaranteed to be non‑negative for the valid recurrence */
        cache[i] = pos - neg;
    }

    return cache[n - 1];
}
