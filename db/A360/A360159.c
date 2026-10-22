/*
 * A360159  a(n) is the sum of divisors of n that are odd squares.
 * https://oeis.org/A360159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of divisors of n that are odd squares (equivalently,
 *            product over odd primes p|n of (p^(2(f+1))−1)/(p^2−1), where f =
 *            floor(e_p(n)/2).)
 * Range:     u8 n <= 24, u16 n <= 1443, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* ignore factor 2; it contributes 1 to every term */
    idx_t m = n;
    while ((m & 1) == 0)
        m >>= 1;

    seq_t result = 1;

    /* factor the odd part */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0)
            continue;

        /* exponent of p in n */
        idx_t exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }

        /* largest even exponent ≤ exp */
        idx_t f = exp / 2;          /* floor(exp/2) */
        if (f == 0)
            continue;               /* term equals 1 */

        /* compute (p^(2*(f+1)) - 1) / (p^2 - 1) */
        idx_t two_f_plus1 = 2 * (f + 1);
        seq_t p_pow = big_pow((seq_t)p, (seq_t)two_f_plus1); /* p^(2*(f+1)) */
        seq_t numerator = p_pow - 1;
        seq_t p2 = (seq_t)p * (seq_t)p;
        seq_t denom = p2 - 1;
        seq_t term = numerator / denom;

        result *= term;
    }

    /* if a prime factor > sqrt(original_m) remains, it has exponent 1,
       contributing a factor of 1 to the product (since f=0). */
    return result;
}
