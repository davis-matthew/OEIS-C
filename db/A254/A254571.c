/*
 * A254571  Least multiplier k such that k*n is abundant or perfect (A023196).
 * https://oeis.org/A254571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 8508, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least multiplier k (1,2,3,4,6) such that k*n is perfect or abundant. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Sum of all positive divisors of a 64‑bit integer m.
 * Uses trial division; results fit into uint64_t for the ranges tested. */
static seq_t sigma_u64(uint64_t m)
{
    uint64_t sum = 1;
    uint64_t t = m;

    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p)
            continue;

        uint64_t term = 1;      /* p⁰ */
        uint64_t p_pow = 1;

        while (t % p == 0) {
            t /= p;
            p_pow *= p;          /* pⁱ */
            term += p_pow;       /* accumulate pⁱ */
        }
        sum *= term;
    }

    if (t > 1)                       /* remaining prime factor > sqrt(m) */
        sum *= (1 + t);

    return (seq_t)sum;
}

/* a(n): least k such that k*n is abundant or perfect. */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined for n<1 */
        return (seq_t)0;

    const uint32_t cand[5] = {1, 2, 3, 4, 6};

    for (size_t i = 0; i < 5; ++i) {
        uint32_t k = cand[i];
        uint64_t m = (uint64_t)k * n;

        seq_t sigma = sigma_u64(m);
        seq_t twice = (seq_t)2 * (seq_t)m;

        if (sigma >= twice)
            return (seq_t)k;
    }

    /* By construction one of the candidates satisfies the property. */
    return (seq_t)6;
}
