/*
 * A393589  Expansion of 1/((1-z)*(1-2*z)*(1-3*z))^4.
 * https://oeis.org/A393589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Expansion of 1/((1-z)*(1-2*z)*(1-3*z))^4
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* Positive coefficients of the 12‑term recurrence
   a(n) = 24*a(n-1) - 260*a(n-2) + 1680*a(n-3) - 7206*a(n-4) + ... */
static const seq_t coeff_abs[13] = {
    0,    24,   260, 1680, 7206, 21600, 46364, 71760, 79441, 61320,
    31320, 9504, 1296
};

/* Small integer power, works for builtin integers and arbitrary‑precision ints. */
static seq_t pow_small(seq_t base, int exp) {
    seq_t result = 1;
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}

/* Stirling number of the second kind S(n,3) for n >= 3. */
static seq_t stirling_three(idx_t n) {
    seq_t p3 = pow_small(3, (int)n);
    seq_t p2 = pow_small(2, (int)n);
    seq_t numerator = p3 - 3 * p2 + 3;
    return numerator / 6;
}

/* Compute the n‑th term directly from the definition
   (suitable for 0 <= n <= 11). */
static seq_t compute_direct(idx_t n) {
    seq_t total = 0;
    for (idx_t a = 0; a <= n; ++a) {
        seq_t va = stirling_three(a + 3);
        for (idx_t b = 0; b <= n - a; ++b) {
            seq_t vb = va * stirling_three(b + 3);
            for (idx_t c = 0; c <= n - a - b; ++c) {
                seq_t vc = vb * stirling_three(c + 3);
                idx_t d = n - a - b - c;
                seq_t vd = vc * stirling_three(d + 3);
                total += vd;
            }
        }
    }
    return total;
}

/* Ensure cached array is large enough. */
static void ensure_capacity(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise cache with the first 12 terms computed directly. */
static void init_cache(void) {
    ensure_capacity(16);          /* at least 12, but a bit larger */
    for (idx_t i = 0; i < 12; ++i)
        cache[i] = compute_direct(i);
    cache_len = 12;
}

/* Main function: returns a(n). */
seq_t a(idx_t n) {
    if (cache_len == 0) init_cache();

    if ((idx_t)n < cache_len) return cache[n];

    /* Grow cache to n+1 if necessary. */
    ensure_capacity((size_t)n + 1);

    /* Compute new terms using the 12‑term linear recurrence. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1; k <= 12; ++k) {
            seq_t term = cache[i - k] * coeff_abs[k];
            if (k & 1)          /* k odd → positive coefficient */
                pos += term;
            else                /* k even → negative coefficient */
                neg += term;
        }
        cache[i] = pos - neg;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
