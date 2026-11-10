/*
 * A246717  Numbers of the form 2n - 1 such that A246702(n) = 2.
 * https://oeis.org/A246717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: odd m = 2k-1 >= 3 such that floor((m^2-1)/ord_{m^2}(2))
 *            = 2, i.e. A246702((m+1)/2) = 2
 * Range:     u8 n <= 25, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: standard, pass, 87 terms through n = 105 (2026-11-07)
 * Helpers:   gcd_u64, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/powmod_u64.h"
#include "../utils/gcd_u64.h"

/* multiplicative order of 2 modulo an odd prime p */
static uint64_t A246717_ord_prime(uint64_t p)
{
    uint64_t ord = p - 1, x = p - 1;
    for (uint64_t q = 2; q * q <= x; q += (q == 2 ? 1 : 2)) {
        if (x % q == 0) {
            while (x % q == 0) x /= q;
            while (ord % q == 0 && powmod_u64(2, ord / q, p) == 1) ord /= q;
        }
    }
    if (x > 1) {
        uint64_t q = x;
        while (ord % q == 0 && powmod_u64(2, ord / q, p) == 1) ord /= q;
    }
    return ord;
}

/* true iff floor((m^2-1)/ord_{m^2}(2)) == 2, for odd m >= 3 */
static bool A246717_test(uint64_t m)
{
    uint64_t x = m, L = 1;
    for (uint64_t d = 3; x > 1; d += 2) {
        uint64_t p;
        if (d * d > x) p = x;
        else if (x % d != 0) continue;
        else p = d;
        uint64_t pk = 1;
        while (x % p == 0) { x /= p; pk *= p * p; }
        uint64_t t = A246717_ord_prime(p);
        while (powmod_u64(2, t, pk) != 1) t *= p;
        uint64_t g = gcd_u64(L, t);
        L = (L / g) * t;
    }
    return (m * m - 1) / L == 2;
}

static uint64_t *A246717_cache = NULL;
static size_t A246717_len = 0, A246717_cap = 0;
static uint64_t A246717_next = 3;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A246717_len < (size_t)n) {
        uint64_t m = A246717_next;
        A246717_next += 2;
        if (!A246717_test(m)) continue;
        if (A246717_len == A246717_cap) {
            size_t nc = A246717_cap ? A246717_cap * 2 : 64;
            uint64_t *t = (uint64_t *)realloc(A246717_cache, nc * sizeof(uint64_t));
            if (!t) exit(EXIT_FAILURE);
            A246717_cache = t;
            A246717_cap = nc;
        }
        A246717_cache[A246717_len++] = m;
    }
    return (seq_t)A246717_cache[n - 1];
}
