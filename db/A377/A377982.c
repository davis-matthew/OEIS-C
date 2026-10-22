/*
 * A377982  a(n) is the number of terms of A048103 (numbers with no p^p
 *          divisors) less than or equal to n.
 * https://oeis.org/A377982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { m ≤ n : m has no divisor p^p for any prime p }
 * Range:     u8 n <= 353, u16 n <= 9672, u32 n <= 9672, u64 n <= 9672,
 *            u128 n <= 9672, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (statement derived from a(n) = Σ_{i=0..n} A359550(i) for the OEIS entry) */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* Compute base^exp but stop if the result would exceed limit. */
static idx_t pow_limit(idx_t base, idx_t exp, idx_t limit)
{
    idx_t r = 1;
    for (idx_t i = 0; i < exp; ++i) {
        if (r > limit / base) return limit + 1; /* overflow or exceeds limit */
        r *= base;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* --- gather primes with p^p ≤ n --- */
    idx_t primes[10];
    idx_t  factors[10];
    size_t pcnt = 0;
    for (idx_t p = 2; ; ++p) {
        if (!is_prime(p)) continue;
        idx_t f = pow_limit(p, p, n);
        if (f > n) break;
        primes[pcnt]   = p;
        factors[pcnt]  = f;
        ++pcnt;
    }

    seq_t sum = (seq_t)n;

    size_t total = (size_t)1 << pcnt; /* pcnt ≤ 6 for n ≤ 2^64-1 */
    for (size_t mask = 1; mask < total; ++mask) {
        idx_t prod = 1;
        size_t bits = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            if (mask & (1ULL << i)) {
                ++bits;
                if (prod > n / factors[i]) { prod = n + 1; break; }
                prod *= factors[i];
            }
        }
        if (prod > n) continue;
        seq_t term = (seq_t)(n / prod);
        if (bits & 1) sum -= term;
        else          sum += term;
    }

    return sum;
}
