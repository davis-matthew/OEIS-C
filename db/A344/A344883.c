/*
 * A344883  Numbers k, not a power of prime, such that for all their proper
 *          divisors d A011772(d) < A011772(k).
 * https://oeis.org/A344883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k>1, not a prime power, with A011772(k/p) <
 *            A011772(k) for every prime p | k (equivalent to all proper
 *            divisors d, since d | k implies A011772(d) <= A011772(k)); found
 *            by search
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A011772_s(uint64_t k)
{
    uint64_t lim = 2 * k;
    for (uint64_t m = 1; m < lim; ++m) {
        uint64_t t = (m % 2 == 0) ? (m / 2) * (m + 1) : m * ((m + 1) / 2);
        if (t % k == 0) return m;
    }
    return lim - 1;
}

static int A344883_ok(uint64_t k)
{
    uint64_t primes[32];
    int cnt = 0;
    uint64_t x = k;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            primes[cnt++] = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) primes[cnt++] = x;
    if (cnt < 2) return 0;
    uint64_t sk = A011772_s(k);
    for (int i = 0; i < cnt; ++i) {
        if (A011772_s(k / primes[i]) >= sk) return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t count = 0, cap = 0;
    static uint64_t next_k = 2;
    if (n == 0) return 0;
    while (count < (size_t)n) {
        if (A344883_ok(next_k)) {
            if (count == cap) {
                cap = cap ? cap * 2 : 64;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[count++] = next_k;
        }
        ++next_k;
    }
    return (seq_t)terms[n - 1];
}
