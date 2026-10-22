/*
 * A356574  a(n) = Sum_{d|n} tau(d^4), where tau(n) = number of divisors of n,
 *          cf. A000005.
 * https://oeis.org/A356574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = tau(n) * tau(n^2) = ∏_{p^e‖n} (e+1)(2e+1)
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A356574 – multiplicative with a(p^e) = 2e² + 3e + 1 */

#include <stddef.h>
#include <stdint.h>

/* Integer square‑root for unsigned 64‑bit numbers */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == 0)           /* sequence starts at offset 1; behaviour below is undefined */
        return 0;

    idx_t tmp = n;
    seq_t result = 1;

    /* Factorise n by trial division up to its integer square‑root */
    idx_t limit = isqrt_u64(n);
    for (idx_t p = 2; p <= limit; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* Multiply (e+1) and (2e+1) into the result */
            result *= (seq_t)(e + 1) * (seq_t)(2 * e + 1);
        }
    }

    /* If a prime factor remains (> sqrt(original n)) */
    if (tmp > 1) {
        /* Remaining factor is prime, exponent e = 1 */
        result *= (seq_t)(1 + 1) * (seq_t)(2 * 1 + 1);  /* equals 6 */
    }

    return result;
}
