/*
 * A268322  Number of length-(n+1) 0..3 arrays with new values introduced in
 *          sequential order, and with new repeated values introduced in
 *          sequential order, both starting with zero.
 * https://oeis.org/A268322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A268322 – number of length-(n+1) 0..3 arrays with new values introduced
 * in sequential order and new repeated values introduced in sequential order,
 * both starting with zero.
 *
 * Formula:   a(n) = 15*a(n-1) - 83*a(n-2) + 181*a(n-3) + 20*a(n-4)
 *            - 632*a(n-5) + 445*a(n-6) + 849*a(n-7) - 637*a(n-8)
 *            - 673*a(n-9) + 208*a(n-10) + 260*a(n-11) + 48*a(n-12)
 *
 * Initial values:
 *   a(1)=2, a(2)=4, a(3)=11, a(4)=32, a(5)=102, a(6)=337, a(7)=1148,
 *   a(8)=3984, a(9)=14030, a(10)=49973, a(11)=179735, a(12)=652010
 */

#include <stdlib.h>

/* Cached values of the sequence (1‑based index stored at 0‑based array slot). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;    /* allocated number of entries in cache */
static idx_t computed_up_to = 0; /* maximum n already computed */

/* Initial values of the sequence (1‑based). */
static const seq_t init_vals[12] = {
    2,   4,   11,  32,   102,  337,  1148, 3984, 14030, 49973,
    179735, 652010
};

/* Recurrence coefficients for a(n‑1) … a(n‑12). */
static const int coeffs[12] = {
    15,  -83,   181,    20,  -632,    445,    849,   -637,
   -673,   208,   260,     48
};

/* Ensure that cache can hold at least n entries. */
static void ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < (size_t)n)
            new_cap <<= 1;
        void *p = realloc(cache, new_cap * sizeof(seq_t));
        cache = (seq_t *)p;
        cache_cap = new_cap;
    }
}

/* Main function that returns a(n).  n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* safety – offset is 1 */

    ensure_cache(n);

    /* Fill initial 12 values if not already done. */
    if (computed_up_to < 12) {
        for (int i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
        computed_up_to = 12;
    }

    /* Compute terms up to n if needed. */
    if (computed_up_to < n) {
        for (idx_t i = computed_up_to + 1; i <= n; ++i) {
            /* All terms i <= 12 are already known. */
            if (i <= 12) {
                computed_up_to = i;
                continue;
            }

            /* Compute a(i) using the recurrence. */
            if (sizeof(seq_t) <= sizeof(unsigned long long)) {
                unsigned __int128 pos = 0, neg = 0;
                for (int k = 0; k < 12; ++k) {
                    idx_t idx_term = i - (idx_t)k - 2;   /* a(i‑k‑1) */
                    seq_t term = cache[idx_term];
                    unsigned int coeff_abs = coeffs[k] >= 0 ? coeffs[k] : -coeffs[k];

                    unsigned __int128 prod = (unsigned __int128)term * coeff_abs;
                    if (coeffs[k] >= 0)
                        pos += prod;
                    else
                        neg += prod;
                }
                cache[i - 1] = (seq_t)(pos - neg);
            } else {
                seq_t pos = 0, neg = 0;
                for (int k = 0; k < 12; ++k) {
                    idx_t idx_term = i - (idx_t)k - 2;   /* a(i‑k‑1) */
                    seq_t term = cache[idx_term];
                    unsigned int coeff_abs = coeffs[k] >= 0 ? coeffs[k] : -coeffs[k];

                    if (coeffs[k] >= 0)
                        pos += term * (seq_t)coeff_abs;
                    else
                        neg += term * (seq_t)coeff_abs;
                }
                cache[i - 1] = pos - neg;
            }
            computed_up_to = i;
        }
    }

    return cache[n - 1];
}
