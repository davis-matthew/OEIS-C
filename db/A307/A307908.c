/*
 * A307908  a(n) is the least k such that p^k >= n for any prime factor p of
 *          n.
 * https://oeis.org/A307908
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that p^k ≥ n for every prime factor p of n *
 *            Implementation uses trial factorisation with the helper *
 *            `smallest_prime_factor` from utils to obtain each prime divisor,
 *            then * determines the smallest exponent k for that prime by
 *            repeated multiplication. * The maximum k over all...
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 88 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"

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
    /* Undefined for n < 2; return 0 to satisfy type requirements. */
    if (n < 2) return (seq_t)0;

    uint64_t temp = (uint64_t)n;   /* copy of n for factorisation */
    int maxk = 0;                  /* maximum exponent seen */

    while (temp > 1) {
        /* get one prime factor of temp */
        uint64_t p = smallest_prime_factor(temp);

        /* compute smallest k with p^k >= n using idx_t for safety */
        idx_t val = 1;
        int k = 0;
        while (val < (idx_t)n) {
            val *= (idx_t)p;
            ++k;
        }
        if (k > maxk) maxk = k;

        /* remove all copies of prime p from temp */
        while (temp % p == 0) temp /= p;
    }

    return (seq_t)maxk;
}
