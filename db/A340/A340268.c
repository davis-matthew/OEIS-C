/*
 * A340268  Composite numbers k>1 such that (s-1) | (d-1) for each d | k,
 *          where s = lpf(k) = A020639(k).
 * https://oeis.org/A340268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   Return the nth composite integer k>1 such that
   for every prime factor p of k,
     (p-1) is a multiple of (s-1),
   where s=lpf(k) is the smallest prime factor of k.
   Even composites (s=2) and numbers whose smallest prime is 3
   automatically satisfy the property, so they are accepted without
   further testing.  all other composites are checked by trial
   division.  The function stores results in a static cache so it
   works for arbitrary access order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool _ok_composite(uint64_t n)
{
    if (n < 4) return false;          /* not composite */
    /* find smallest prime factor s */
    uint64_t s = 0;
    if (n % 2 == 0) s = 2;
    else if (n % 3 == 0) s = 3;
    else {
        for (uint64_t p = 5; p * p <= n; p += 6) {
            if (n % p == 0) { s = p; break; }
            if (n % (p + 2) == 0) { s = p + 2; break; }
        }
        if (s == 0) s = n; /* n is prime */
    }
    if (s == n) return false; /* prime */

    /* For s <= 3 the property holds for all composites */
    if (s <= 3) return true;

    uint64_t m = s - 1;   /* divisor to test against */

    /* Factor n, checking distinct primes */
    uint64_t rest = n;
    uint64_t prev = 0;
    while (rest > 1) {
        uint64_t p = 0;
        /* Find next prime factor */
        if (rest % 2 == 0) p = 2;
        else if (rest % 3 == 0) p = 3;
        else {
            for (uint64_t q = 5; q * q <= rest; q += 6) {
                if (rest % q == 0) { p = q; break; }
                if (rest % (q + 2) == 0) { p = q + 2; break; }
            }
            if (p == 0) p = rest; /* rest is prime */
        }

        /* Check condition for distinct primes */
        if (p != prev) {
            if (p != s && (p - 1) % m != 0) return false;
            prev = p;
        }
        while (rest % p == 0) rest /= p;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t sz    = 0;
    static uint64_t next_k = 4; /* smallest composite */

    while (sz < n) {
        if (_ok_composite(next_k)) {
            /* Grow cache if necessary */
            if (sz == cap) {
                size_t newcap = cap == 0 ? 16 : cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                if (!tmp) return 0; /* allocation failure */
                cache = tmp;
                cap   = newcap;
            }
            cache[sz++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
