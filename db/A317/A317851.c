/*
 * A317851  Primes p such that A090086(p+1) < p.
 * https://oeis.org/A317851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p such that A090086(p+1) < p; found by search
 *            over primes and composite x < p with (p+1)^(x-1) == 1 (mod x)
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool A317851_isprime(uint64_t n)
{
    if (n < 2) return false;
    for (uint64_t d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}

static uint64_t A317851_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = (uint64_t)(((unsigned __int128)r * b) % m);
        b = (uint64_t)(((unsigned __int128)b * b) % m);
        e >>= 1;
    }
    return r;
}

/* true if smallest pseudoprime to base p+1 is < p */
static bool A317851_test(uint64_t p)
{
    uint64_t b = p + 1;
    for (uint64_t x = 4; x < p; ++x) {
        if (A317851_powmod(b, x - 1, x) == 1 && !A317851_isprime(x))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    for (uint64_t p = 2; ; ++p) {
        if (!A317851_isprime(p)) continue;
        if (A317851_test(p)) {
            if (++count == (uint64_t)n) return (seq_t)p;
        }
    }
}
