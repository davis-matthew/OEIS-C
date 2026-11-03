/*
 * A308317  Multiplicative with a(prime(k)^e) = A005117(e+1)^(2^(k-1)).
 * https://oeis.org/A308317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 12, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-01)
 * Helpers:   ipow_i32, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A308317
 *  a(n) is multiplicative with
 *      a(p_k^e) = A005117(e)^(2^(k-1)),
 *  where p_k is the k-th prime, e >= 1.
 *  a(1) = 1.
 *
 *  The implementation factors n by trial division, determines the
 *  exponent e and the prime index k of each prime factor using
 *  compute_pi(), obtains the non‑square value via a small
 *  non_square() helper (based on floor_sqrt), raises it to the
 *  power 2^(k-1) by repeated squaring, and multiplies the results.
 */

#include <stdint.h>

#include <stddef.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/prime_pi.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return the m‑th positive integer that is not a perfect square.
 *   m >= 1
 */
static seq_t non_square(idx_t m)
{
    seq_t x = (seq_t)1;
    /* Count of nonsquares <= x is x - floor_sqrt(x) */
    while ((x - floor_sqrt(x)) < (seq_t)m)
        ++x;
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* sequence is 1‑based; value undefined for 0 */
        return (seq_t)0;

    seq_t result = (seq_t)1;
    uint64_t temp = (uint64_t)n;

    /* Factorise n by trial division */
    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* Determine exponent e of prime p */
            idx_t e = 0;
            while (temp % p == 0) {
                ++e;
                temp /= p;
            }

            /* Base is A005117(e) = e-th non‑square */
            seq_t base = non_square(e);

            /* Prime index k = number of primes <= p */
            idx_t k = (idx_t)prime_pi(p);

            /* Compute base^(2^(k-1)) by repeated squaring */
            seq_t mult = base;
            for (idx_t i = 1; i < k; ++i)
                mult = mult * mult;

            result = result * mult;
        }
    }

    /* Remaining prime factor > sqrt(original n), if any */
    if (temp > 1) {
        uint64_t p = temp;
        idx_t e = 1;                 /* exponent is 1 */
        seq_t base = non_square(e);
        idx_t k = (idx_t)prime_pi(p);
        seq_t mult = base;
        for (idx_t i = 1; i < k; ++i)
            mult = mult * mult;
        result = result * mult;
    }

    return result;
}
