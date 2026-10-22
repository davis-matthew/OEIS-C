/*
 * A338625  a(1) = 1; for n > 1, ceiling(a(n-1)/2)+3 if a(n-1) and n are
 *          coprime, otherwise a(n) = GCD(a(n-1),n)*2+1.
 * https://oeis.org/A338625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; For n > 1: a(n) = ceil(a(n-1)/2)+3 if gcd(a(n-1),n)=1,
 *            else a(n) = gcd(a(n-1),n)*2+1
 * Range:     u8 n <= 349, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static idx_t capacity = 0;
static idx_t max_computed = 0;

static void ensure_capacity(idx_t needed)
{
    if (needed <= capacity) return;
    size_t new_cap = needed > capacity ? (capacity == 0 ? 4 : capacity * 2) : needed;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – abort (no other recovery possible in this context) */
        abort();
    }
    cache = tmp;
    capacity = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but guard anyway */
    if (n > max_computed) {
        ensure_capacity(n);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i == 1) {
                cache[0] = (seq_t)1;
            } else {
                seq_t prev = cache[i - 2];
                seq_t g = gcd(prev, (seq_t)i);
                seq_t term;
                if (g == (seq_t)1) {
                    term = (prev + (seq_t)1) / (seq_t)2 + (seq_t)3;
                } else {
                    term = g * (seq_t)2 + (seq_t)1;
                }
                cache[i - 1] = term;
            }
        }
        max_computed = n;
    }
    return cache[n - 1];
}
