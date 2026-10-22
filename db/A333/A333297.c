/*
 * A333297  a(n) = Sum_{i=1..n, j=1..n, gcd(i,j)=1} i.
 * https://oeis.org/A333297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=2..n} 3*k*phi(k)/2 (with a(1)=1)
 * Range:     u8 n <= 8, u16 n <= 59, u32 n <= 2416, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/phi.h"   /* provides seq_t phi(idx_t m) */

/* Cached values of the sequence: a_cache[i] = a(i), i >= 1 */
static seq_t *a_cache = NULL;
static idx_t a_cached_until = 0;

/* Extend the cache to include at least index n (n >= 1). */
static void extend_a(idx_t n)
{
    if (n <= a_cached_until)
        return;   /* already computed */

    /* Allocate memory for indices 0..n (index 0 unused) */
    seq_t *new_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * (n + 1));
    if (!new_cache)
        abort();
    a_cache = new_cache;

    /* Compute missing terms */
    for (idx_t i = a_cached_until + 1; i <= n; ++i) {
        if (i == 1) {
            a_cache[1] = (seq_t)1;
        } else {
            seq_t phi_val = phi(i);
            seq_t term = (seq_t)3 * (seq_t)i * phi_val / (seq_t)2;
            a_cache[i] = a_cache[i - 1] + term;
        }
    }
    a_cached_until = n;
}

/* Public function computing the sequence */
seq_t a(idx_t n)
{
    if (n < 1)         /* offset protection – undefined for n==0 */
        return 0;

    extend_a(n);
    return a_cache[n];
}
