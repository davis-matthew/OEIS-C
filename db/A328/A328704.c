/*
 * A328704  Primes dividing nonzero terms in A002065.
 * https://oeis.org/A328704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p (increasing) such that p divides some
 *            A002065(k), k>=1, i.e. 0 lies on a cycle of x -> x^2+x+1 mod p;
 *            found by a search over primes
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A328704_divides_some_term(uint64_t p) {
    uint64_t x = 0;
    for (uint64_t i = 0; i < p; i++) {
        x = (x * x + x + 1) % p;
        if (x == 0) return true;
    }
    return false;
}

static bool A328704_isprime(uint64_t p) {
    if (p < 2) return false;
    for (uint64_t d = 2; d * d <= p; d++)
        if (p % d == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    static uint64_t cache[4096];
    static idx_t cnt = 0;
    static uint64_t next = 2;
    while (cnt < n && cnt < 4096) {
        uint64_t p = next++;
        if (A328704_isprime(p) && A328704_divides_some_term(p))
            cache[cnt++] = p;
    }
    return (seq_t)cache[n - 1];
}
