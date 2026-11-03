/*
 * A373381  a(n) = gcd(bigomega(n), A056239(n)), where bigomega is number of
 *          prime factors with repetition, and A056239 is fully additive with
 *          a(p) = primepi(p).
 * https://oeis.org/A373381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A001222(n), A056239(n))
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9617 (2026-11-03)
 * Helpers:   bigomega, gcd, gcd_u64, is_prime_u64, mulmod, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/gcd.h"
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/prime_pi.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition; for n >= 1 the following holds */
    if (n < 1) return 0;   /* defensive guard for n == 0 */

    /* A001222(n) – number of prime factors with multiplicity */
    seq_t bigom = bigomega(n);

    /* A056239(n) – fully additive with a(p) = π(p) (primepi(p)) */
    seq_t sumA = 0;
    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t p  = smallest_prime_factor(m);
        uint64_t cnt = 0;

        while (m % p == 0) {
            m /= p;
            ++cnt;
        }

        seq_t pi_p = prime_pi(p);
        sumA += (seq_t)cnt * pi_p;
    }

    return gcd(bigom, sumA);
}
