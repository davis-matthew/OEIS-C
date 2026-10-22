/*
 * A367514  The exponentially odious part of n: the largest unitary divisor of
 *          n that is an exponentially odious number (A270428).
 * https://oeis.org/A367514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n, popcount(e) odd} p^e (largest unitary divisor
 *            whose prime‑power exponents are odious)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* handle factor 2 */
    if ((m & 1ULL) == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        if (__builtin_popcountll(e) & 1u) {
            seq_t term = 1;
            for (uint64_t i = 0; i < e; ++i) {
                term *= (seq_t)2;
            }
            result *= term;
        }
    }

    /* handle odd factors */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (__builtin_popcountll(e) & 1u) {
                seq_t term = 1;
                for (uint64_t i = 0; i < e; ++i) {
                    term *= (seq_t)p;
                }
                result *= term;
            }
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        result *= (seq_t)m;
    }

    return result;
}
