/*
 * A400277  a(n) = phi(n)^2 mod (n+1), where phi is Euler's totient function
 *          (A000010).
 * https://oeis.org/A400277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = phi(n)^2 mod (n+1)
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute Euler's totient function for n as a 64‑bit unsigned integer */
static uint64_t phi_uint64(idx_t n)
{
    if (n <= 1) return 1;
    uint64_t result = n;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            result -= result / p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) result -= result / temp;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not used in OEIS offset */
    uint64_t mod   = (uint64_t)n + 1;     /* n+1 fits in 64‑bit */
    uint64_t phi_n = phi_uint64(n);      /* totient value (≤ n) */
    uint64_t phi_m = phi_n % mod;        /* reduce modulo (n+1) */
    uint64_t res   = (phi_m * phi_m) % mod;  /* square and reduce */
    return (seq_t)res;                   /* cast to generated seq_t type */
}
