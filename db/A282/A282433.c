/*
 * A282433  Coefficients in q-expansion of E_6^5, where E_6 is the Eisenstein
 *          series A013973.
 * https://oeis.org/A282433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 3, i64 n <= 7, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A282433  
   Coefficients in the q‑expansion of E_6^5, where  
   E_6(q) = 1 - 504 ∑_{m≥1} σ_5(m) q^m  
   (σ_5(m) = ∑_{d|m} d^5).  
   a(n) is the coefficient of q^n in (E_6(q))^5. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* External linkage as required */
extern seq_t a(idx_t n);

/* Convolution of two truncated power series:  C = A * B  (mod q^cap) */
static void conv(const seq_t *A, const seq_t *B, seq_t *C, size_t cap)
{
    for (size_t i = 0; i < cap; ++i) C[i] = 0;
    for (size_t i = 0; i < cap; ++i) {
        if (A[i] == 0) continue;
        seq_t ai = A[i];
        size_t jmax = cap - i;
        for (size_t j = 0; j < jmax; ++j) {
            if (B[j] == 0) continue;
            C[i + j] += ai * B[j];
        }
    }
}

extern seq_t a(idx_t n)
{
    /* Cache of already computed coefficients */
    static seq_t *cache      = NULL;   /* a[0 .. cache_cap-1]          */
    static size_t cache_cap  = 0;       /* number of cached elements     */

    size_t need = (size_t)(n + 1);
    if (need > cache_cap) {
        /* Grow to next power of two at least 'need' */
        size_t new_cap = (cache_cap == 0 ? 1 : cache_cap);
        while (new_cap < need) new_cap <<= 1;

        /* Temporary arrays */
        seq_t *sigma5 = (seq_t *)calloc(new_cap, sizeof(seq_t));
        seq_t *x      = (seq_t *)calloc(new_cap, sizeof(seq_t));
        seq_t *xpow2  = (seq_t *)calloc(new_cap, sizeof(seq_t));
        seq_t *xpow3  = (seq_t *)calloc(new_cap, sizeof(seq_t));
        seq_t *xpow4  = (seq_t *)calloc(new_cap, sizeof(seq_t));
        seq_t *xpow5  = (seq_t *)calloc(new_cap, sizeof(seq_t));

        if (!sigma5 || !x || !xpow2 || !xpow3 || !xpow4 || !xpow5) {
            abort();  /* memory allocation failure */
        }

        /* Compute σ5(m) for m < new_cap */
        for (size_t d = 1; d < new_cap; ++d) {
            seq_t d5 = 1;
            for (int k = 0; k < 5; ++k) d5 *= (seq_t)d;
            for (size_t m = d; m < new_cap; m += d) sigma5[m] += d5;
        }

        /* Build x(q) = -504 ∑ σ5(m) q^m   (x[0] = 0) */
        x[0] = 0;
        for (size_t i = 1; i < new_cap; ++i) x[i] = (seq_t)-504 * sigma5[i];

        /* Powers of x: xpow2 = x^2, ..., xpow5 = x^5 */
        conv(x, x, xpow2, new_cap);     /* x^2 */
        conv(xpow2, x, xpow3, new_cap); /* x^3 */
        conv(xpow3, x, xpow4, new_cap); /* x^4 */
        conv(xpow4, x, xpow5, new_cap); /* x^5 */

        /* Binomial coefficients for (1 + x)^5 */
        const seq_t binom[6] = {1, 5, 10, 10, 5, 1};

        /* Assemble new cache */
        seq_t *new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
        if (!new_cache) abort();

        new_cache[0] = 1;  /* constant term from (1)^5 */

        for (size_t i = 0; i < new_cap; ++i) {
            new_cache[i] += binom[1] * x[i];
            new_cache[i] += binom[2] * xpow2[i];
            new_cache[i] += binom[3] * xpow3[i];
            new_cache[i] += binom[4] * xpow4[i];
            new_cache[i] += binom[5] * xpow5[i];
        }

        /* Clean up temporaries */
        free(sigma5);
        free(x);
        free(xpow2);
        free(xpow3);
        free(xpow4);
        free(xpow5);
        free(cache);

        cache      = new_cache;
        cache_cap  = new_cap;
    }

    return cache[(size_t)n];
}
