/*
 * A394679  Expansion of 2F1(1/4, 3/4; 1/2; 4*x/(1-x)^2)^2.
 * https://oeis.org/A394679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [xⁿ] ₂F₁(¼,¾;½;4x/(1‑x)²)². Recurrence: (2n+3)(n+4)a(n+4)
 *            – 6(4n²+19n+20)a(n+3) + 4(19n²+76n+75)a(n+2) –
 *            6(4n²+13n+8)a(n+1) + n(2n+5)a(n) = 0 Initial values: a(0)=1,
 *            a(1)=3, a(2)=17, a(3)=95
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   ensure_cache_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/ensure_cache_size.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = (idx_t)-1;   /* indicates 'not yet initialised' */

    /* Ensure enough space */
    ensure_cache_size(n + 1, &cache, &cache_cap);

    /* Initialise base values once */
    if (cache_max == (idx_t)-1) {
        ensure_cache_size(4, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)17;
        cache[3] = (seq_t)95;
        cache_max = 3;
    }

    /* Return cached value if already computed */
    if (n <= cache_max) return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        /* In the recurrence we compute a(k) from a(k‑1)…a(k‑4) */
        idx_t i = k - 4;

        seq_t past0 = cache[i];     /* a(i)   */
        seq_t past1 = cache[i + 1]; /* a(i+1) */
        seq_t past2 = cache[i + 2]; /* a(i+2) */
        seq_t past3 = cache[i + 3]; /* a(i+3) */

        seq_t res;
        /* Use 128‑bit intermediate if seq_t is 64‑bit or smaller,
           otherwise use seq_t arithmetic directly. */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            unsigned __int128 I  = (unsigned __int128)i;
            unsigned __int128 I2 = I * I;

            /* Coefficients expressed in 128‑bit arithmetic */
            unsigned __int128 c1 = 6 * ((unsigned __int128)4 * I2
                                    + (unsigned __int128)19 * I + 20);
            unsigned __int128 c2 = 4 * ((unsigned __int128)19 * I2
                                    + (unsigned __int128)76 * I + 75);
            unsigned __int128 c3 = 6 * ((unsigned __int128)4 * I2
                                    + (unsigned __int128)13 * I + 8);
            unsigned __int128 c4 = I * ((unsigned __int128)2 * I + 5);

            unsigned __int128 term1 = c1 * (unsigned __int128)past3;
            unsigned __int128 term2 = c2 * (unsigned __int128)past2;
            unsigned __int128 term3 = c3 * (unsigned __int128)past1;
            unsigned __int128 term4 = c4 * (unsigned __int128)past0;

            unsigned __int128 num = term1 - term2 + term3 - term4;
            unsigned __int128 denom = ((unsigned __int128)2 * I + 3)
                                    * (I + 4);

            res = (seq_t)(num / denom);
        } else {
            /* seq_t is an arbitrary‑precision type */
            seq_t I   = (seq_t)i;
            seq_t I2  = I * I;

            seq_t c1 = 6 * (4 * I2 + 19 * I + 20);
            seq_t c2 = 4 * (19 * I2 + 76 * I + 75);
            seq_t c3 = 6 * (4 * I2 + 13 * I + 8);
            seq_t c4 = I * (2 * I + 5);

            seq_t num = c1 * past3 - c2 * past2 + c3 * past1 - c4 * past0;
            seq_t denom = (2 * I + 3) * (I + 4);
            res = num / denom;
        }

        cache[k] = res;
    }

    cache_max = n;
    return cache[n];
}
