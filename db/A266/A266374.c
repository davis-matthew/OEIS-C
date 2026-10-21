/*
 * A266374  G.f. = b(2)*b(6)/(3*x^6-2*x^5-2*x+1), where b(k) = (1-x^k)/(1-x).
 * https://oeis.org/A266374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of x^n in G.f. =
 *            b(2)*b(6)/(3*x^6-2*x^5-2*x+1), where b(k) = (1-x^k)/(1-x)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 61, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence derived from (3*x^6-2*x^5-2*x+1)*G(x)=b(2)*b(6) is
   a(n) = 2*a(n-1) + 2*a(n-5) - 3*a(n-6) for n>=7,
   with initial values a(0)=1, a(1)=4, a(2)=10, a(3)=22,
   a(4)=46, a(5)=96, a(6)=198. */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

// Initial values for n = 0..6
static const seq_t init_vals[7] = {
    (seq_t)1,   // a(0)
    (seq_t)4,   // a(1)
    (seq_t)10,  // a(2)
    (seq_t)22,  // a(3)
    (seq_t)46,  // a(4)
    (seq_t)96,  // a(5)
    (seq_t)198  // a(6)
};

seq_t a(idx_t n) {
    // Ensure cache large enough to hold all values up to n
    if (cache_len == 0) {
        cache_len = 7;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 7; ++i) cache[i] = init_vals[i];
    }
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            // Allocation failed; fallback to recomputing current n
            tmp = cache;
        } else {
            cache = tmp;
            // Compute missing entries
            for (idx_t i = cache_len; i <= n; ++i) {
                // For i <= 6 initialised already
                if (i <= 6) continue;
                seq_t term = 2 * cache[i-1] + 2 * cache[i-5];
                term -= 3 * cache[i-6];
                cache[i] = term;
            }
            cache_len = new_len;
        }
    }
    return cache[n];
}
