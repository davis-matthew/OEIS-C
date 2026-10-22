/*
 * A378616  Greatest non prime power <= prime(n).
 * https://oeis.org/A378616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest integer ≤ prime(n) that is NOT a prime power. *
 *            The result is prime(n)−1 except when prime(n)−1 itself is a
 *            prime power * (those terms are listed in OEIS A159611)
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"

/* Test whether a 64‑bit integer is a prime power (p^k, k≥1).
 * Returns true for primes, powers of primes, and false otherwise.
 * 1 is not a prime power. */
static bool is_prime_power_uint64(uint64_t x)
{
    if (x <= 1)
        return false;

    uint64_t r = x;
    for (uint64_t p = 2; p * p <= r; ++p) {
        if (r % p == 0) {
            while (r % p == 0)
                r /= p;
            return r == 1;
        }
    }
    /* If we reach here, r has no divisor ≤ sqrt(x).  It is prime => prime power. */
    return true;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 as per OEIS offset. */
    uint32_t pn = get_prime(n);              /* nth prime, fits in 32 bits. */
    seq_t cand = (seq_t)pn - 1;              /* start from one below the prime. */

    for (seq_t v = cand; v > 0; --v) {
        if (!is_prime_power_uint64((uint64_t)v))
            return v;
    }
    return 1; /* Never reached, but keeps compiler happy. */
}
