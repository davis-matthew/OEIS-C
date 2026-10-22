/*
 * A386012  a(n) = n^3*tau(n).
 * https://oeis.org/A386012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^3 * tau(n)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 599, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(n) – number of divisors of n */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0U)            /* undefined for n==0 – return 0 */
        return 0;

    /* compute the divisor count tau(n) by trial division */
    uint64_t temp = n;
    uint64_t tau  = 1;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            tau *= (exp + 1);
        }
    }
    if (temp > 1)          /* remaining prime factor */
        tau *= 2;

    /* compute n^3 * tau(n) */
    seq_t nn      = (seq_t)n;
    seq_t cube    = nn * nn * nn;
    seq_t tau_seq = (seq_t)tau;
    return cube * tau_seq;
}
