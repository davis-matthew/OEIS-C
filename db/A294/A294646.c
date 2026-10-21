/*
 * A294646  a(n) = (1/2)^(2*n) mod (2*n+1).
 * https://oeis.org/A294646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the smallest k > 0 such that k * 2^(2*n) ≡ 1 (mod 2*n+1) */
/* This implementation works for any seq_t and idx_t defined by the build.
   It uses only integer arithmetic and a small integer modulo for efficiency. */

#include <stdint.h>

/* Extended Euclid to compute modular inverse modulo m (m > 1, a coprime to m). */
static uint64_t modinv_uint64(uint64_t a, uint64_t m)
{
    int64_t t0 = 0, t1 = 1;
    int64_t r0 = m, r1 = a % m;

    while (r1 != 0) {
        uint64_t q = r0 / r1;
        uint64_t r = r0 % r1;
        int64_t t = t0 - (int64_t)q * t1;

        r0 = r1; r1 = r;
        t0 = t1; t1 = t;
    }
    if (t0 < 0) t0 += m;
    return (uint64_t)t0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* not defined for n=0, return 0 as safe default */

    /* modulus m = 2*n+1, exponent exp = 2*n */
    uint64_t m = (uint64_t)(2 * n + 1);
    uint64_t exp = (uint64_t)(2 * n);

    /* Compute 2^exp mod m using binary exponentiation with 128‑bit safe mul. */
    uint64_t base = 2 % m;
    uint64_t pow2 = 1 % m;
    while (exp) {
        if (exp & 1U) pow2 = (uint64_t)((__uint128_t)pow2 * base % m);
        base = (uint64_t)((__uint128_t)base * base % m);
        exp >>= 1U;
    }

    /* Compute modular inverse of pow2 modulo m. */
    uint64_t inv = modinv_uint64(pow2, m);

    /* Convert back to seq_t, guaranteed to fit because inv < m. */
    return (seq_t)inv;
}
