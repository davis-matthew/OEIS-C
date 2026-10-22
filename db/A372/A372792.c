/*
 * A372792  Number of divisors of 12n; a(n) = tau(12*n) = A000005(12*n).
 * https://oeis.org/A372792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8745, u16 n <= 8745, u32 n <= 8745, u64 n <= 8745,
 *            u128 n <= 8745, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of divisors of 12*n, a(n) = τ(12*n) */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = n;         /* work on a copy of n */
    uint64_t e2 = 0;        /* exponent of 2 in n */
    uint64_t e3 = 0;        /* exponent of 3 in n */

    /* factor out powers of 2 */
    while ((m & 1ULL) == 0) {
        e2++;
        m >>= 1;
    }

    /* factor out powers of 3 */
    while (m % 3ULL == 0) {
        e3++;
        m /= 3ULL;
    }

    /* exponents in 12*n: 2^2 * 3^1 * n */
    e2 += 2;
    e3 += 1;

    seq_t tau = 1;
    tau = tau * ((seq_t)(e2 + 1));
    tau = tau * ((seq_t)(e3 + 1));

    /* factor remaining primes greater than 3 */
    uint64_t i = 5;
    while (i <= m / i) {
        uint64_t c = 0;
        while (m % i == 0) {
            c++;
            m /= i;
        }
        if (c > 0) {
            tau = tau * ((seq_t)(c + 1));
        }

        uint64_t j = i + 2;
        c = 0;
        while (m % j == 0) {
            c++;
            m /= j;
        }
        if (c > 0) {
            tau = tau * ((seq_t)(c + 1));
        }

        i += 6;
    }

    if (m > 1) {  /* m is a remaining prime factor with exponent 1 */
        tau = tau * ((seq_t)2);
    }

    return tau;
}
