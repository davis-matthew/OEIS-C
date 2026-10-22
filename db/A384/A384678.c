/*
 * A384678  Expansion of (1+x) / (1-2*x-4*x^2+2*x^3).
 * https://oeis.org/A384678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A384678: Expansion of (1+x)/(1-2*x-4*x^2+2*x^3).
 *
 * The generating function satisfies: (1 - 2x - 4x^2 + 2x^3) * f(x) = 1 + x,
 * hence the coefficients obey the linear recurrence
 *   a(n) = 2*a(n-1) + 4*a(n-2) - 2*a(n-3)   for n >= 3
 * with initial values a(0)=1, a(1)=3, a(2)=10.
 *
 * The function computes a(n) for any 0 ≤ n ≤ 20000.  The array is
 * lazily initialized and reused for subsequent calls in any order.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t cache[20001];
static idx_t cache_len = 0;

/* Compute a(i) for i from cache_len to n inclusive. */
static void ensure_cache_to(idx_t n) {
    if (n < cache_len)
        return;                       /* already computed */

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == (idx_t)0) {
            cache[i] = (seq_t)1;
        } else if (i == (idx_t)1) {
            cache[i] = (seq_t)3;
        } else if (i == (idx_t)2) {
            cache[i] = (seq_t)10;
        } else {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            cache[i] = (seq_t)2 * a1 + (seq_t)4 * a2 - (seq_t)2 * a3;
        }
    }
    cache_len = n + 1;
}

seq_t a(idx_t n) {
    if (n >= 20001) {
        /* If n exceeds the preallocated range, compute directly using
           the recurrence without caching (fast for a single large n). */
        seq_t a0 = (seq_t)1;
        seq_t a1 = (seq_t)3;
        seq_t a2 = (seq_t)10;
        if (n == 0) return a0;
        if (n == 1) return a1;
        if (n == 2) return a2;
        for (idx_t i = 3; i <= n; ++i) {
            seq_t an = (seq_t)2 * a2 + (seq_t)4 * a1 - (seq_t)2 * a0;
            a0 = a1;
            a1 = a2;
            a2 = an;
        }
        return a2;
    }
    ensure_cache_to(n);
    return cache[n];
}
