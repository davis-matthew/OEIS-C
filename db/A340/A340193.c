/*
 * A340193  a(n) = n - (Sum_{d|n} A063994(d)).
 * https://oeis.org/A340193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - Sum_{d|n} A063994(d), where A063994(d) =
 *            Product_{primes p|d} gcd(p-1, d-1)
 * Range:     u8 n <= 287, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 107 terms through n = 8067 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A340193_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static seq_t A340193_A063994(uint64_t d)
{
    if (d == 1) return (seq_t)1;
    seq_t prod = 1;
    uint64_t m = d;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            prod *= (seq_t)A340193_gcd(p - 1, d - 1);
        }
    }
    if (m > 1) prod *= (seq_t)A340193_gcd(m - 1, d - 1);
    return prod;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    uint64_t N = (uint64_t)n;
    for (uint64_t i = 1; i * i <= N; i++) {
        if (N % i == 0) {
            sum += A340193_A063994(i);
            uint64_t j = N / i;
            if (j != i) sum += A340193_A063994(j);
        }
    }
    return (seq_t)n - sum;
}
