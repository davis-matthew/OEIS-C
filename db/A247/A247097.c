/*
 * A247097  a(n) = least integer m such that prime(n)+m and prime(n+1)+m are
 *          prime.
 * https://oeis.org/A247097
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least integer m > 0 such that prime(n)+m and prime(n+1)+m
 *            are both prime. The index offset is 2 (a(2) is the first term)
 * Range:     u8 n <= 49, u16 n <= 6157, u32 n <= 6347, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"     /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* For n < 2 the behaviour is undefined; return 0 for safety. */
    if (n < 2) return (seq_t)0;

    /* Obtain the n-th and (n+1)-th primes. */
    uint32_t p   = get_prime(n);
    uint32_t pn1 = get_prime(n + 1);

    /* Search for the smallest even m > 0 such that both sums are prime. */
    seq_t m = 2;                      /* start with the smallest positive even */
    while (1) {
        seq_t q1 = (seq_t)p   + m;    /* prime(n)   + m */
        seq_t q2 = (seq_t)pn1 + m;    /* prime(n+1) + m */

        if (is_prime_big(q1) && is_prime_big(q2))
            return m;

        m += 2;                      /* only even m can work (primes are odd) */
    }
}
