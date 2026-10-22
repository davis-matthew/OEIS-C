/*
 * A370565  Size of the group Q_3*_/(Q_3*)^n, where Q_3 is the field of 3-adic
 *          numbers.
 * https://oeis.org/A370565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * 3^e * gcd(2, n'), where n = 3^e * n' (n' not
 *            divisible by 3)
 * Range:     u8 n <= 17, u16 n <= 485, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local helper: compute 3^exp */
static seq_t pow3_exp(idx_t exp) {
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) {
        r *= (seq_t)3;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* find exponent e of 3 in n: n = 3^e * n' */
    idx_t nn = n;
    idx_t e = 0;
    while ((nn % 3) == 0) {
        nn /= 3;
        ++e;
    }

    seq_t n_prime = (seq_t)nn;

    /* gcd(2, n') is 2 if n' even, otherwise 1 */
    seq_t g = ((n_prime & 1) == 0) ? (seq_t)2 : (seq_t)1;

    /* 3^(2e) */
    seq_t pow3_2e = pow3_exp(e << 1);

    /* assemble result: 3^(2e) * n' * gcd(2, n') */
    seq_t res = pow3_2e;
    res *= n_prime;
    res *= g;
    return res;
}
