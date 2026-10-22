/*
 * A301166  Coordination sequence T1 for Zeolite Code RWY.
 * https://oeis.org/A301166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A301166 – Coordination sequence T1 for Zeolite Code RWY
   Generating function:
       G(x) = (1 + 4x + 6x² + 12x³ + 16x⁴ + 24x⁵ + 30x⁶ + 32x⁷ + 35x⁸ + 32x⁹ + 34x¹⁰ + 24x¹¹
               + 16x¹² + 12x¹³ + 6x¹⁴ + 4x¹⁵)
/          (1 - x⁴)(1 - x⁶)²
   Using D(x) = 1 - x⁴ - 2x⁶ + 2x¹⁰ + x¹² - x¹⁶, the coefficients
   satisfy the linear recurrence:
       a(n) = N(n) + a(n‑4) + 2·a(n‑6) – 2·a(n‑10) – a(n‑12) + a(n‑16),
   where N(n) is the numerator coefficient for n < 16 (0 otherwise).
   Initial values: a(0)=1, a(1)=4, a(2)=6, a(3)=12.  All other terms
   for n<0 are treated as 0.
*/
#include <stdlib.h>

/* Numerator coefficients for n = 0 … 15 */
static const idx_t Ncoeff[16] = {
    1, 4, 6, 12, 16, 24, 30, 32, 35, 32, 34, 24, 16, 12, 6, 4
};

/* Cache for already computed terms */
static seq_t *cache   = NULL; /* array of seq_t values */
static idx_t  cache_n = 0;    /* next index that is not yet computed */
static size_t cache_cap = 0;  /* current allocated capacity */

/* Ensure that cache contains values a(0)..a(n) */
static void ensure_cache(idx_t n)
{
    if (n < cache_n)              /* already computed */
        return;

    /* Grow the cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap > 0 ? cache_cap : 16;
        if (new_cap < needed)
            new_cap = needed;
        while (new_cap < needed)
            new_cap <<= 1;       /* grow by powers of two */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms that are still missing */
    for (idx_t i = cache_n; i <= n; ++i) {
        seq_t pos = 0;   /* positive contributions */
        seq_t neg = 0;   /* negative contributions */

        /* N(i) */
        if (i < 16)
            pos += (seq_t)Ncoeff[i];

        /* Recurrence terms */
        if (i >= 4)
            pos += cache[i - 4];
        if (i >= 6)
            pos += ((seq_t)2) * cache[i - 6];
        if (i >= 16)
            pos += cache[i - 16];

        if (i >= 10)
            neg += ((seq_t)2) * cache[i - 10];
        if (i >= 12)
            neg += cache[i - 12];

        cache[i] = pos - neg;   /* all terms fit in seq_t by definition */
    }
    cache_n = n + 1;            /* all indices up to n are now ready */
}

/* Public interface – returns a(n) for n >= 0 */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
