/*
 * A272077  Primes of the form abs(7*k^2 - 371*k + 4871) in order of
 *          increasing nonnegative values of k.
 * https://oeis.org/A272077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime of the form |7*k^2 - 371*k + 4871| for k ≥
 *            0, returned in order of increasing k
 * Range:     u8 none, u16 n <= 95, u32 n <= 47, u64 n <= 3530, u128 n <= 3530,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t found = 0;
    static idx_t next_k = 0;

    if (n == 0)  /* undefined, but return 0 for safety */
        return 0;

    /* Build cached values up to the requested index */
    while (found < n)
    {
        /* Compute the quadratic expression as a signed 64‑bit value.
           For sequences up to ~10⁴ terms the intermediate values fit
           comfortably in a signed 64‑bit integer. */
        int64_t k  = (int64_t)next_k;
        int64_t v  = 7 * k * k - 371 * k + 4871;
        if (v < 0) v = -v;
        seq_t val = (seq_t)v;

        /* Test for primality */
        if (is_prime_big(val))
        {
            /* Grow cache if needed */
            if (found == cap)
                grow_seq(&cache, &cap, found + 1);
            cache[found++] = val;
        }
        ++next_k;
    }

    return cache[n - 1];
}
