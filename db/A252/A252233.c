/*
 * A252233  Characteristic function for the integers that are the product of
 *          an odd number of primes each with multiplicity one.
 * https://oeis.org/A252233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is square‑free and has an odd number of distinct
 *            prime factors, otherwise a(n) = 0. a(1) = 0
 * Range:     u8 n <= 8678, u16 n <= 8678, u32 n <= 8678, u64 n <= 8678,
 *            u128 n <= 8678, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t m = (uint64_t)n;
    int cnt = 0;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            /* check if p appears with power ≥ 2 */
            if ((m / p) % p == 0) return (seq_t)0;
            ++cnt;
            while (m % p == 0) m /= p;
        }
    }

    if (m > 1) ++cnt;   /* the remaining prime factor */

    return (seq_t)((cnt & 1) ? 1 : 0);
}
