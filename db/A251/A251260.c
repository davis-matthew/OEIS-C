/*
 * A251260  Expansion of (1 + 2*x + x^2 + x^3) / ((1 - x^2)^2 * (1 - x^3) * (1
 *          - x^4)) in powers of x.
 * https://oeis.org/A251260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 151, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A251260:
 *   Coefficients of (1+2*x+x^2+x^3)/((1-x^2)^2*(1-x^3)*(1-x^4))
 * Recurrence (for n ≥ 0):
 *   a(n+11) = 2*a(n+9) + a(n+8) - 2*a(n+6) - 2*a(n+5)
 *            + a(n+3) + 2*a(n+2) - a(n)
 *   Initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=6, a(4)=8,
 *                   a(5)=13, a(6)=16, a(7)=24, a(8)=28,
 *                   a(9)=40, a(10)=45
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static memoisation table */
    static seq_t *cache = NULL;
    static idx_t cap = 0;   /* allocated capacity */
    static idx_t last = 0;  /* highest index filled */

    /* initial coefficients a(0)...a(10) */
    static const seq_t init[11] = { 1, 2, 3, 6, 8, 13, 16, 24, 28, 40, 45 };

    /* perform one‑time initialisation */
    if (cap == 0) {
        /* first capacity larger than the initial block */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;             /* allocation failure → zero */
        for (idx_t i = 0; i < 11; ++i) cache[i] = init[i];
        for (idx_t i = 11; i < cap; ++i) cache[i] = 0;
        last = 10;
    }

    /* enlarge cache if needed */
    if (n >= cap) {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure → zero */
        cache = tmp;
        for (idx_t i = cap; i < newcap; ++i) cache[i] = 0;
        cap = newcap;
    }

    /* compute new terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i <= 10) continue;        /* already initialized */

            /* recurrence: a(i) = 2*a(i-2)+a(i-3)+a(i-8)+2*a(i-9)
                               -a(i-11)-2*a(i-5)-2*a(i-6) */
            seq_t val = 0;

            /* positive contributions */
            val += cache[i-9];
            val += cache[i-9];
            val += cache[i-8];
            val += cache[i-3];
            val += cache[i-2];
            val += cache[i-2];

            /* negative contributions */
            val -= cache[i-11];
            val -= cache[i-6];
            val -= cache[i-6];
            val -= cache[i-5];
            val -= cache[i-5];

            cache[i] = val;
        }
        last = n;
    }

    return cache[n];
}
