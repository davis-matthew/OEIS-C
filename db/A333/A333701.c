/*
 * A333701  Number of ways to write n as the sum of two positive integers with
 *          the same number of distinct prime factors.
 * https://oeis.org/A333701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} [omega(i) = omega(n-i)]
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-03)
 * Helpers:   gcd_u64, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where omega(x) is the number of distinct prime factors of x. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

/* Compute omega (number of distinct prime factors) for a single integer. */
static uint8_t _omega(uint64_t n)
{
    if (n <= 1) return 0;
    uint8_t cnt = 0;
    uint64_t last = 0;
    while (n > 1) {
        uint64_t p = smallest_prime_factor(n);
        if (p != last) {
            cnt++;
            last = p;
        }
        while (n % p == 0) n /= p;
    }
    return cnt;
}

/* Cache of omega values up to a certain maximum index. */
static uint8_t *omega_cache = NULL;
static idx_t omega_max = 0;

/* Extend the cache so that positions 0..new_max are valid. */
static void ensure_omega(idx_t new_max)
{
    if (new_max <= omega_max) return;
    uint8_t *tmp = (uint8_t *)realloc(omega_cache, (size_t)(new_max + 1) * sizeof(uint8_t));
    if (!tmp) return;                /* fail silently – values 0 will be treated as valid */
    for (idx_t i = omega_max + 1; i <= new_max; ++i)
        tmp[i] = _omega((uint64_t)i);
    omega_cache = tmp;
    omega_max = new_max;
}

/* Main OEIS function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* value outside the offset is irrelevant */
    ensure_omega(n);
    seq_t result = 0;
    for (idx_t i = 1; i <= n / 2; ++i) {
        if (omega_cache[i] == omega_cache[n - i]) result += 1;
    }
    return result;
}
