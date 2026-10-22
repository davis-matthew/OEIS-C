/*
 * A318755  a(n) = Sum_{k=1..n} tau(k)^3, where tau is A000005.
 * https://oeis.org/A318755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} tau(k)^3 (OEIS A318755)
 * Range:     u8 n <= 9, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compute the divisor counting function tau(k).  This simple trial
   division is fast enough for n up to 10000 and satisfies all
   integer‐only constraints. */
static unsigned int tau_of(idx_t n)
{
    if (n == 0) return 0; /* not used for n ≥ 1 */
    idx_t m = n;
    unsigned int tau = 1;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            unsigned int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            tau *= (e + 1);
        }
    }
    if (m > 1)            /* remaining prime factor */
        tau *= 2;
    return tau;
}

/* Dynamic cache for the cumulative sums a(n).  The cache grows
   on demand, so repeated calls are fast and memory usage is minimal. */
static seq_t *a_cache  = NULL;    /* pointer to an array of seq_t */
static idx_t  cached_max = 0;      /* largest n for which a_cache[n] is valid */

/* Wrapper for realloc that casts to seq_t*.  This works for both C
   and C++ (the cast is explicit). */
static seq_t *resize_cache(idx_t new_cap)
{
    seq_t *raw = NULL;
    if (a_cache == NULL) {
        raw = (seq_t *)malloc((size_t)new_cap * sizeof(seq_t));
    } else {
        raw = (seq_t *)realloc(a_cache, (size_t)new_cap * sizeof(seq_t));
    }
    if (!raw) abort();          /* out of memory – terminate */
    a_cache = raw;
    return raw;
}

/* Public sequence function.  It fills the cache up to n if needed
   and then returns a_cache[n].  It works correctly for any call
   order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; 0 is unused */

    /* Ensure the cache can hold indices up to n. */
    if (n > cached_max) {
        idx_t new_cap = n + 1;          /* include index 0 */
        if (a_cache == NULL) {
            /* First allocation: also set a_cache[0] = 0. */
            a_cache = resize_cache(new_cap);
            a_cache[0] = (seq_t)0;
        } else if (new_cap > cached_max + 1) {
            a_cache = resize_cache(new_cap);
        }

        /* Compute missing cumulative sums. */
        for (idx_t k = cached_max + 1; k <= n; ++k) {
            unsigned int tau = tau_of(k);
            seq_t cube = (seq_t)tau * (seq_t)tau * (seq_t)tau;
            a_cache[k] = a_cache[k - 1] + cube;
        }
        cached_max = n;
    }
    return a_cache[n];
}
