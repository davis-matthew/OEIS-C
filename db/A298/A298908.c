/*
 * A298908  Smallest composite k such that (n^k - 1)/(n - 1) == 1 (mod k) for
 *          n > 1.
 * https://oeis.org/A298908
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A298908:
   a(n) = the smallest composite k such that
   (n^k - 1)/(n - 1) ≡ 1 (mod k),
   i.e. sum_{i=0}^{k-1} n^i ≡ 1 (mod k).  n > 1. */

#include <stdbool.h>
#include <stdint.h>

/* Return true if k is composite (k ≥ 4).  Basic trial division. */
static bool is_composite(idx_t k)
{
    if (k < 4)
        return false;
    for (idx_t d = 2; d <= k / d; ++d) {
        if (k % d == 0)
            return true;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure n > 1 by definition of the sequence. */
    for (idx_t k = 2; ; ++k) {
        if (!is_composite(k))
            continue;               /* skip primes and <4 */

        uint64_t modk = (uint64_t)k;  /* modulo value */
        uint64_t sum  = 0;
        uint64_t pow  = 1;
        uint64_t sn   = (uint64_t)n;

        /* Compute sum_{i=0}^{k-1} n^i  (mod k) */
        for (idx_t i = 0; i < k; ++i) {
            sum += pow;
            if (sum >= modk)
                sum %= modk;
            pow = (pow * sn) % modk;
        }

        if (sum % modk == 1)
            return (seq_t)k;        /* found the smallest composite */
    }
}
