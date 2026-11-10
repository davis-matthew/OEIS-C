/*
 * A353417  a(n) = A113415(A252463(n)), where A113415 is the arithmetic mean
 *          between the number of odd divisors and their sum, and A252463 is
 *          the hybrid shift.
 * https://oeis.org/A353417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (d(m) + sigma(m))/2 over odd divisors of m = A252463(n);
 *            A252463(1)=1, A252463(2k)=k, A252463(odd) = prod prev_prime(p)^e
 * Range:     u8 n <= 520, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 106 terms through n = 9503 (2026-11-10)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t A353417_prev_prime(uint64_t p)
{
    uint64_t q = p - 1;
    while (q > 1 && !is_prime((idx_t)q)) --q;
    return q;
}

static uint64_t A353417_hybrid(uint64_t n)
{
    if (n == 1) return 1;
    if ((n & 1u) == 0) return n >> 1;
    uint64_t m = n, r = 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t q = A353417_prev_prime(p);
            while (m % p == 0) { m /= p; r *= q; }
        }
    }
    if (m > 1) r *= A353417_prev_prime(m);
    return r;
}

seq_t a(idx_t n)
{
    uint64_t m = A353417_hybrid((uint64_t)n);
    while ((m & 1u) == 0) m >>= 1;
    uint64_t d = 1;
    seq_t s = (seq_t)1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            seq_t term = (seq_t)1, sum = (seq_t)1;
            while (m % p == 0) {
                m /= p;
                ++e;
                term *= (seq_t)p;
                sum += term;
            }
            d *= (e + 1);
            s *= sum;
        }
    }
    if (m > 1) {
        d *= 2;
        s *= ((seq_t)m + 1);
    }
    return ((seq_t)d + s) / 2;
}
