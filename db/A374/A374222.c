/*
 * A374222  a(n) = 1 if sigma(n) and sopfr(n) are both multiples of 3,
 *          otherwise 0, where sigma is the sum of divisors, and sopfr is the
 *          sum of prime factors with repetition.
 * https://oeis.org/A374222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(n) and sopfr(n) are both multiples of 3,
 *            otherwise 0
 * Range:     u8 n <= 125, u16 n <= 9689, u32 n <= 9689, u64 n <= 9689,
 *            u128 n <= 9689, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses naive factorisation; sufficient for n ≤ 10⁴ and
   runs well under 2 s per call on the benchmark. */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to a standard 64‑bit unsigned for internal loops */
    uint64_t m = (uint64_t)n;

    seq_t sigma  = 0;
    seq_t sopfr  = 0;

    /* --- Compute sigma(n): sum of divisors --- */
    for (uint64_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            sigma += (seq_t)i;
            uint64_t j = m / i;
            if (j != i) sigma += (seq_t)j;
        }
    }

    /* --- Compute sopfr(n): sum of prime factors with multiplicity --- */
    uint64_t temp = m;
    while (temp > 1) {
        uint64_t sp = temp;            /* assume temp itself is prime */
        for (uint64_t d = 2; d * d <= temp; ++d) {
            if (temp % d == 0) {
                sp = d;
                break;
            }
        }
        sopfr += (seq_t)sp;
        temp /= sp;
    }

    /* Return 1 iff both sigma and sopfr are divisible by 3. */
    return ((sigma % 3 == 0) && (sopfr % 3 == 0)) ? (seq_t)1 : (seq_t)0;
}
