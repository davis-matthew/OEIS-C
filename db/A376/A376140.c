/*
 * A376140  The number of divisors of n whose prime factorization has maximum
 *          exponent that is smaller than the maximum exponent in the prime
 *          factorization of n.
 * https://oeis.org/A376140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{p^e∥n} (e + [e < max_e]) where max_e is * the
 *            maximum exponent in the prime factorization of n, with a(1)=0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    uint64_t tmp = (uint64_t)n;
    uint64_t exps[64];
    size_t    excnt = 0;
    uint64_t  p = 2;

    /* trial division; avoid overflow in p*p by using p <= tmp/p */
    while (p <= tmp / p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            exps[excnt++] = e;
        }
        p = (p == 2) ? 3 : p + 2;  /* test 2 then odd numbers */
    }
    if (tmp > 1) exps[excnt++] = 1;   /* remaining prime factor */

    /* find maximum exponent */
    uint64_t max_e = 0;
    for (size_t i = 0; i < excnt; ++i)
        if (exps[i] > max_e)
            max_e = exps[i];

    /* compute the product of (e + [e < max_e]) */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < excnt; ++i) {
        seq_t factor = (seq_t)exps[i];
        if (exps[i] < max_e)
            factor = factor + (seq_t)1;
        result *= factor;
    }

    return result;
}
