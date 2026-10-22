/*
 * A339871  Number of primes p for which the p-adic valuation of phi(n)
 *          exceeds the p-adic valuation of n-1, with a(1) = 0 by convention.
 * https://oeis.org/A339871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{p prime : v_p(phi(n)) > v_p(n‑1)}|
 * Range:     u8 n <= 530, u16 n <= 9750, u32 n <= 9750, u64 n <= 9750,
 *            u128 n <= 9750, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;          /* a(1) = 0 by convention */

    /* Euler's totient of n */
    seq_t phi_val = phi((idx_t)n);

    /* Factor φ(n) into primes and exponents (φ(n) ≤ 10⁴ for n ≤ 10⁴) */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)phi_val, primes, exps, &cnt);

    seq_t result = (seq_t)0;
    const uint64_t nm1 = (uint64_t)n - 1;  /* n-1 > 0 for n > 1 */

    /* For each prime divisor of φ(n) check the p‑adic inequality */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t vp_nm1 = 0;
        uint64_t tmp = nm1;
        while (tmp % p == 0) {
            tmp /= p;
            ++vp_nm1;
        }
        if (exps[i] > vp_nm1) {
            ++result;
        }
    }
    return result;
}
