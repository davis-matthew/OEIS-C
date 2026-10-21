/*
 * A241531  a(n) = smallest k>=1 such that 2*prime(n) - k^2 is semiprime (or 0
 *          if there is no such k).
 * https://oeis.org/A241531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   _smallest_prime_factor, get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest k≥1 such that 2·prime(n) – k^2 is a semiprime */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/is_prime_small.h"

static inline bool is_semiprime(uint64_t x)
{
    /* A semiprime is a composite number with exactly two prime factors
       (counting multiplicity).  This routine checks that efficiently. */
    if (x < 4)                    /* 4=2*2 is the smallest semiprime */
        return false;
    uint64_t sp = _smallest_prime_factor(x); /* smallest prime factor */
    if (sp == 0 || sp == x)      /* x is 1 or prime */
        return false;
    uint64_t t = x / sp;
    return is_prime_small(t);     /* n = sp · t and t is prime ⇒ semiprime */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* sequence is undefined below offset 1 */

    uint64_t p   = get_prime(n);          /* prime(n) */
    uint64_t base = 2ULL * p;              /* 2·prime(n) */

    /* Search k = 1,2,3,… while base – k^2 ≥ 2 (the smallest semiprime). */
    for (uint64_t k = 1; k * k <= base - 2; ++k)
    {
        uint64_t N = base - k * k;
        if (is_semiprime(N))
            return (seq_t)k;              /* found the smallest such k */
    }

    return 0;                            /* no suitable k found */
}
