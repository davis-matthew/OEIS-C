/*
 * A262568  a(n) = A002703(n) + 2.
 * https://oeis.org/A262568
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/m) * Σ_{d|m} φ(d) * Kronecker(2,d) * 2^((m/d-1)/2)
 *            where m = 2n+1. For odd m we have Kronecker(2,d)=+1 if d≡1 or 7
 *            (mod 8) and –1 otherwise
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers – available for both C11 and C++17.  */
#include <stdint.h>
#include <stdbool.h>

/* Helper: Euler's totient for an odd integer d≤20001.  */
static inline seq_t phi_uint64(uint64_t d)
{
    uint64_t result = d;
    for (uint64_t p = 2; p * p <= d; ++p) {
        if (d % p == 0) {
            while (d % p == 0) d /= p;
            result -= result / p;
        }
    }
    if (d > 1) result -= result / d;
    return (seq_t)result;
}

/* Helper: compute 2^k as seq_t.  For k large and seq_t small the value
   will overflow naturally (mod 2^w) – acceptable for uint64_t builds
   whose tests only cover terms that fit. */
static inline seq_t pow2_uint64(idx_t k)
{
    seq_t r = 1;
    for (idx_t i = 0; i < k; ++i) r *= 2;
    return r;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined for indices <3 */

    idx_t m = 2 * n + 1;          /* odd modulus */
    seq_t sum = 0;                /* accumulating Σ φ(d) * K * 2^k */

    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d != 0) continue;

        idx_t d1 = d;            /* first divisor */
        idx_t d2 = m / d;        /* complementary divisor */

        /* --- process divisor d1 --- */
        uint64_t d1_u = (uint64_t)d1;
        seq_t phi1 = phi_uint64(d1_u);
        int sign1 = ((d1 % 8 == 1) || (d1 % 8 == 7)) ? 1 : -1;
        idx_t k1 = (d2 - 1) / 2;
        seq_t pow1 = pow2_uint64(k1);
        seq_t term1 = phi1 * pow1;
        sum += (sign1 > 0) ? term1 : -term1;

        /* --- process complementary divisor d2, if distinct --- */
        if (d2 != d1) {
            uint64_t d2_u = (uint64_t)d2;
            seq_t phi2 = phi_uint64(d2_u);
            int sign2 = ((d2 % 8 == 1) || (d2 % 8 == 7)) ? 1 : -1;
            idx_t k2 = (d1 - 1) / 2;
            seq_t pow2 = pow2_uint64(k2);
            seq_t term2 = phi2 * pow2;
            sum += (sign2 > 0) ? term2 : -term2;
        }
    }

    /* Final result = sum / m.  Division is exact by the formula. */
    return sum / (seq_t)m;
}
