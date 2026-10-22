/*
 * A334739  Number of unordered factorizations of n with 2 different parts >
 *          1.
 * https://oeis.org/A334739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number of unordered factorizations of n with exactly two different
   parts > 1 equals the number of distinct pairs (x , y) of integers > 1,
   x < y, together with the number of positive integer solutions (i , j)
   of n = x^i · y^j.  For each divisor x of n, iterate over all
   powers x^i that divide n; for every remainder r = n / x^i > 1,
   enumerate all integer bases y such that r = y^k for some k ≥ 1.
   Every such pair (x , y) with y > x contributes one factorization. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd_uint64.h"

/* Count all integer bases y > x such that y^k = rem for some k ≥ 1.
   Each such y corresponds to one unordered factorization of n. */
static void count_roots(const uint64_t rem, const uint64_t x, seq_t *cnt)
{
    if (rem <= 1) return;
    if (rem <= x) return;               /* will be handled by the caller if rem > x */

    /* Factor rem.  The maximum number of prime factors of a 64‑bit integer
       is less than 20, so a small fixed buffer is sufficient. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt;

    factor_u64(rem, primes, exps, &pcnt);

    /* Compute the greatest common divisor of the exponents. */
    uint64_t g = exps[0];
    for (size_t i = 1; i < pcnt; ++i) {
        g = gcd_uint64(g, exps[i]);
    }

    /* Enumerate all divisors k of g (including k = 1). */
    for (uint64_t k = 1; k <= g; ++k) {
        if (g % k != 0) continue;
        /* Compute y = product primes[i]^(exps[i]/k). */
        uint64_t y = 1;
        for (size_t i = 0; i < pcnt; ++i) {
            uint64_t exp = exps[i] / k;
            for (uint64_t t = 0; t < exp; ++t) {
                y *= primes[i];
            }
        }
        if (y > x) {
            (*cnt) += 1;
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    const uint64_t N = (uint64_t)n;
    seq_t total = 0;

    /* Enumerate all divisors x > 1 of N. */
    for (uint64_t x = 2; x <= N - 1; ++x) {
        if (N % x != 0) continue;        /* x must be a divisor */

        /* Iterate over all powers of x: x^i divides N. */
        uint64_t power = x;
        while (power <= N && N % power == 0) {
            uint64_t rem = N / power;
            if (rem > 1) {
                /* For this remainder rem, enumerate all y such that y^k = rem.
                   Each such y > x gives a distinct factorization. */
                count_roots(rem, x, &total);
            }

            /* Prevent overflow in the next multiplication. */
            if (power > N / x) break;
            power *= x;
        }
    }

    return total;
}
