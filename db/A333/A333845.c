/*
 * A333845  a(1) = a(3) = 0, and otherwise a(n) is the least multiple of
 *          prime(n) whose decimal representation ends with that of
 *          prime(n+1).
 * https://oeis.org/A333845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least positive integer that is a multiple of prime(n) and
 *            whose * decimal representation ends in the decimal
 *            representation of prime(n+1). * If no such integer exists, a(n)
 *            = 0
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   digit_count, gcd_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/digit_count.h"
#include "../utils/gcd_u64.h"

/* 64‑bit modular inverse (extended gcd).  a and m must be coprime. */
static uint64_t modinv_u64(uint64_t a, uint64_t m)
{
    int64_t t0 = 0, t1 = 1;
    int64_t r0 = m, r1 = a;
    while (r1 != 0) {
        uint64_t q = r0 / r1;
        int64_t  r = r0 - q * r1;
        int64_t  t = t0 - q * t1;
        r0 = r1;  r1 = r;
        t0 = t1;  t1 = t;
    }
    /* r0 holds gcd(a,m).  It must be 1.  Normalize t0 to [0,m). */
    if (t0 < 0) t0 += m;
    return (uint64_t)t0;
}

/* 64‑bit greatest common divisor. */

/* Sequence function. */
seq_t a(idx_t n)
{
    /* Prime(n) and Prime(n+1).  get_prime returns a uint32_t. */
    uint64_t p = (uint64_t)get_prime(n);
    uint64_t q = (uint64_t)get_prime(n + 1);

    /* Base 10 modulus: 10^d, where d = number of decimal digits of q. */
    uint64_t d = digit_count(q);
    uint64_t mod = 1;
    for (uint64_t i = 0; i < d; ++i) mod *= 10ULL;   /* mod <= 10^6 for the value range */

    /* Solve p * k ≡ q (mod mod). */
    uint64_t g = gcd_u64(p, mod);
    if (q % g != 0) return (seq_t)0;    /* no solution → a(n)=0 */

    uint64_t p1 = p / g;
    uint64_t m1 = mod / g;
    uint64_t q1 = q / g;

    uint64_t inv = modinv_u64(p1 % m1, m1);
    uint64_t k   = (q1 % m1) * inv % m1;   /* smallest positive k */

    seq_t result = (seq_t)p * (seq_t)k;
    return result;
}
