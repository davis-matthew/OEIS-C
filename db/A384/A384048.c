/*
 * A384048  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is a unitary divisor of n is squarefree.
 * https://oeis.org/A384048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e||n} (p if e = 1 p^e – 1 if e ≥ 2)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* external linkage function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;          /* a(1) = 1 */

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* trial division up to √m, avoiding overflow:  p ≤ m/p */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            seq_t prime_power = (seq_t)1;

            /* compute p^exp */
            while (m % p == 0) {
                m /= p;
                exp++;
                prime_power *= (seq_t)p;
            }

            if (exp == 1)            /* p^1 contributes p itself          */
                result *= prime_power;
            else                       /* p^e contributes p^e – 1          */
                result *= (prime_power - (seq_t)1);
        }
    }

    /* if a prime factor > √original n remains */
    if (m > 1)                 /* remaining factor has exponent 1  */
        result *= (seq_t)m;

    return result;
}
