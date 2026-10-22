/*
 * A347870  a(n) = A003415(sigma(n)) mod 2, where A003415 is the arithmetic
 *          derivative, and sigma is the sum of divisors of n.
 * https://oeis.org/A347870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(sigma(n)) mod 2 Computed by factoring sigma(n)
 *            and evaluating parity of its arithmetic derivative
 * Range:     u8 n <= 9545, u16 n <= 9545, u32 n <= 9545, u64 n <= 9545,
 *            u128 n <= 9545, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for out‑of‑range calls */

    /* -------- compute sigma(n) -------- */
    uint64_t N = (uint64_t)n;
    uint64_t sigma = 0;

    for (uint64_t d = 1; d <= N / d; ++d) {
        if (N % d == 0) {
            sigma += d;                   /* add divisor d */
            uint64_t e = N / d;           /* complementary divisor */
            if (e != d) sigma += e;       /* prevent double counting */
        }
    }

    /* -------- parity of arithmetic derivative of sigma -------- */
    uint64_t m = sigma;
    int e0 = 0;                            /* exponent of 2 in sigma */
    while ((m & 1ULL) == 0) {              /* while m is even  */
        ++e0;
        m >>= 1;                            /* divide by 2 */
    }

    int parity = 0;

    /* contribution of the prime 2 */
    if ((e0 & 1) && e0 == 1) {             /* only when e0 is odd and exactly 1 */
        parity ^= 1;
    }

    /* factor the remaining odd part */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {            /* count exponent of p */
                ++e;
                m /= p;
            }
            if ((e & 1) && e0 == 0) {       /* odd exponent & sigma odd */
                parity ^= 1;
            }
        }
    }

    /* leftover prime factor if any */
    if (m > 1) {
        int e = 1;                          /* exponent is 1 */
        if ((e & 1) && e0 == 0) {
            parity ^= 1;
        }
    }

    return (seq_t)parity;
}
