/*
 * A276094  a(n) = n modulo A002110(A257993(n)), a(0) = 0.
 * https://oeis.org/A276094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n mod A002110(A257993(n)), a(0) = 0; A257993(n) = index
 *            of least prime not dividing n, A002110 = primorial
 * Range:     u8 n <= 419, u16 n <= 2310, u32 n <= 2310, u64 n <= 2310,
 *            u128 n <= 2310, big any n
 * Validated: standard, pass, 126 terms through n = 2201 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static const uint64_t primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53
    };
    uint64_t m = (uint64_t)n;
    uint64_t prod = 1;
    size_t i;
    if (m == 0)
        return (seq_t)0;
    for (i = 0; i < sizeof(primes) / sizeof(primes[0]); i++) {
        uint64_t p = primes[i];
        if (prod > UINT64_MAX / p) {
            /* primorial exceeds 64 bits, so it exceeds n */
            return (seq_t)m;
        }
        prod *= p;
        if (m % p != 0)
            return (seq_t)(m % prod);
    }
    return (seq_t)m;
}
