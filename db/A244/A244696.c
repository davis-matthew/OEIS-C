/*
 * A244696  Number of length n+4 0..4 arrays with no five consecutive elements
 *          with pattern ababa or abbba (with a!=b) and new values 0..4
 *          introduced in 0..4 order.
 * https://oeis.org/A244696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) -14*a(n-2) -9*a(n-3) +15*a(n-4) +7*a(n-5)
 *            +22*a(n-6) -9*a(n-7) +9*a(n-8) +13*a(n-9) -9*a(n-10) -17*a(n-12)
 *            -5*a(n-14) -6*a(n-15) -a(n-16) -3*a(n-17) +a(n-18) -a(n-19)
 *            Initial values for n=1..20 are encoded in the static array below
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A244696. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Initial 20 terms (a(1) .. a(20)). */
    static const seq_t init[20] = {
        50ULL,
        191ULL,
        801ULL,
        3582ULL,
        16673ULL,
        79535ULL,
        384984ULL,
        1879575ULL,
        9222654ULL,
        45385845ULL,
        223728645ULL,
        1103952328ULL,
        5450384180ULL,
        26918312059ULL,
        132969494589ULL,
        656908143379ULL,
        3245528085657ULL,
        16035496065995ULL,
        79229859441587ULL,
        391472138972739ULL
    };

    /* Return directly for n <= 20. */
    if (n <= 20)
        return init[n - 1];

    /* Extension cache: grow as needed, memoize results. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (cache == NULL) {
        /* Initially load first 20 terms into cache. */
        cache_len = 20;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        memcpy(cache, init, sizeof(init));
    }

    /* Grow cache if we need to compute beyond current size. */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;            /* Out‑of‑memory: return zero. */
        cache = tmp;
        cache_len = new_len;
    }

    /* Recurrence coefficients & offsets:
       Positive contributions: +8*a(n-1), +15*a(n-4), +7*a(n-5),
       +22*a(n-6), +9*a(n-8), +13*a(n-9), +1*a(n-18).
       Negative contributions: -14*a(n-2), -9*a(n-3), -9*a(n-7),
       -9*a(n-10), -17*a(n-12), -5*a(n-14), -6*a(n-15),
       -1*a(n-16), -3*a(n-17), -1*a(n-19). */
    const int pos_off[7]  = {1, 4, 5, 6, 8, 9, 18};
    const int pos_coef[7] = {8,15,7,22,9,13,1};
    const int neg_off[10] = {2, 3, 7,10,12,14,15,16,17,19};
    const int neg_coef[10] = {14,9,9,9,17,5,6,1,3,1};

    /* Compute missing terms using recurrence. */
    for (idx_t idx = 20; idx < n; ++idx) {          /* idx is 0‑based, a(idx+1) */
        seq_t pos = 0;
        seq_t neg = 0;
        for (int i = 0; i < 7; ++i) {
            idx_t k = pos_off[i];
            pos += cache[idx - k] * (seq_t)pos_coef[i];
        }
        for (int i = 0; i < 10; ++i) {
            idx_t k = neg_off[i];
            neg += cache[idx - k] * (seq_t)neg_coef[i];
        }
        cache[idx] = pos - neg;   /* pos >= neg for all n >= 20 */
    }

    return cache[n - 1];
}
