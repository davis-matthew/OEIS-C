/*
 * A250804  Number of (7+1)X(n+1) 0..1 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -4*a(n-2) -17*a(n-3) +29*a(n-4) +16*a(n-5)
 *            -56*a(n-6) +6*a(n-7) +49*a(n-8) -19*a(n-9) -20*a(n-10)
 *            +11*a(n-11) +3*a(n-12) -2*a(n-13) for n > 13 Initial values:
 *            a(1)=1970, a(2)=9956, a(3)=37986, a(4)=124366, a(5)=361344,
 *            a(6)=968942, a(7)=2437366, a(8)=5849556, a(9)=13519146,
 *            a(10)=...
 * Range:     u8 none, u16 n <= 3, u32 n <= 16, u64 n <= 48, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Defined with external linkage as requested. */
#include <stdlib.h>

static inline seq_t mul_const(seq_t a, unsigned int m)
{
    /* For built‐in arithmetic types, this uses overflow semantics.
       For arbitrary-precision types (e.g. class with operator *),
       the multiplication is performed exactly. */
    return a * (seq_t)m;
}

seq_t a(idx_t n)
{
    /* 0‑based index in cache; a(n) is stored in cache[n‑1] */
    static seq_t *cache = NULL;
    static idx_t cached_terms = 0;

    /* First term (n==0) is undefined; return 0 for safety. */
    if (n == 0) return (seq_t)0;

    /* Ensure cache holds at least the first 13 terms */
    if (cached_terms == 0) {
        idx_t needed = n > 13 ? n : 13;
        cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */

        static const seq_t init[13] = {
            (seq_t)1970,   (seq_t)9956,   (seq_t)37986,
            (seq_t)124366, (seq_t)361344, (seq_t)968942,
            (seq_t)2437366,(seq_t)5849556,(seq_t)13519146,
            (seq_t)30351918,(seq_t)66573728,(seq_t)143373294,
            (seq_t)304256334
        };
        for (idx_t i = 0; i < 13; ++i) cache[i] = init[i];
        cached_terms = 13;
    }

    /* Extend cache if necessary */
    if (n > cached_terms) {
        /* Grow to at least n terms */
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failure */
        cache = tmp;

        for (idx_t i = cached_terms; i < n; ++i) {
            /* cache[i] stores a(i+1) */
            seq_t val = 0;
            val = val + mul_const(cache[i-1], 5);
            val = val - mul_const(cache[i-2], 4);
            val = val - mul_const(cache[i-3], 17);
            val = val + mul_const(cache[i-4], 29);
            val = val + mul_const(cache[i-5], 16);
            val = val - mul_const(cache[i-6], 56);
            val = val + mul_const(cache[i-7], 6);
            val = val + mul_const(cache[i-8], 49);
            val = val - mul_const(cache[i-9], 19);
            val = val - mul_const(cache[i-10], 20);
            val = val + mul_const(cache[i-11], 11);
            val = val + mul_const(cache[i-12], 3);
            val = val - mul_const(cache[i-13], 2);
            cache[i] = val;
        }
        cached_terms = n;
    }

    return cache[n-1];
}
