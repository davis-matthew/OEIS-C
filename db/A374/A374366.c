/*
 * A374366  a(n) = Im(Sum_{d|n} mu(d)*(i^d)).
 * https://oeis.org/A374366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Im( Σ_{d|n} μ(d)·i^d )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Computes a(n) for n ≥ 1.
   The sum runs over all odd square‑free divisors of n. */
seq_t a(idx_t n)
{
    /* a(0) is not used – any value is harmless */
    if (n == 0) return 0;

    uint64_t tmp = (uint64_t)n;

    /* Collect distinct odd prime factors of n */
    uint64_t factors[64];
    size_t k = 0;

    /* Remove all factors of 2 – they do not contribute to odd divisors */
    while ((tmp & 1ULL) == 0) tmp >>= 1;

    /* Trial division by odd primes up to √tmp */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            factors[k++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1)          /* remaining prime factor (odd)    */
        factors[k++] = tmp;

    /* Sum over all odd square‑free divisors.
       The divisor 1 contributes +1. */
    int64_t sum = 1;      /* contribution of d = 1 */

    /* Enumerate all non‑empty subsets of the factors -> all odd square‑free divisors >1 */
    for (uint64_t mask = 1; mask < (uint64_t)(1ULL << k); ++mask) {
        uint64_t d = 1;
        int bits = 0;
        for (size_t j = 0; j < k; ++j) {
            if (mask & (1ULL << j)) {
                d *= factors[j];
                ++bits;
            }
        }
        int mu   = (bits & 1) ? -1 : 1;          /* μ(d) = (-1)^bits      */
        int sign = (d & 3ULL) == 1 ? 1 : -1;     /* Im(i^d)              */
        sum += mu * sign;
    }

    return (seq_t)sum;
}
