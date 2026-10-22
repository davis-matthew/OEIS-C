/*
 * A333046  a(1) = 1; a(n) = n * Sum_{d|n, d < n, gcd(d, n/d) = 1} a(d) / d.
 * https://oeis.org/A333046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, changed
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 909, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1;
   a(n)=n*Σ_{d|n, d<n, gcd(d,n/d)=1} a(d)/d */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;          /* one-based cache */
static uint8_t *cached = NULL;       /* flags: 0 = not computed, 1 = computed */
static idx_t cache_cap = 0;

static void ensure_cap(idx_t n)
{
    if (n + 1 <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap <= n) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cached = (uint8_t *)realloc(cached, new_cap * sizeof(uint8_t));
    for (idx_t i = cache_cap; i < new_cap; ++i) cached[i] = 0;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, just return 0 */
    ensure_cap(n);
    if (n == 1) {
        if (!cached[1]) { cache[1] = 1; cached[1] = 1; }
        return cache[1];
    }
    if (cached[n]) return cache[n];

    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t t = n / d;
            /* divisor d */
            if (d < n && gcd((seq_t)d, (seq_t)t) == 1) {
                seq_t val = a(d);
                sum += val / (seq_t)d;
            }
            /* co-divisor t (avoid double counting) */
            if (t != d && t < n && gcd((seq_t)t, (seq_t)d) == 1) {
                seq_t val = a(t);
                sum += val / (seq_t)t;
            }
        }
    }
    seq_t result = (seq_t)n * sum;
    cache[n] = result;
    cached[n] = 1;
    return result;
}
