/*
 * A381639  Denominators of Sum_{i=1..omega(n)-1} p_{i}/p_{i+1}, where
 *          omega(n) = A001221(n) and p_1 < p_2 < ... p_omega(n) are the
 *          distinct prime factors of n; a(1) = 1.
 * https://oeis.org/A381639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of Σ_{i=1}^{ω(n)-1} p_i / p_{i+1} where p_i
 *            are the distinct prime factors of n sorted increasingly. If ω(n)
 *            < 2 (i.e., n = 1 or prime) the sum is empty and a(n) = 1
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* handle n = 1 and primes directly */
    if (n < 2)
        return (seq_t)1;

    /* buffer for prime factors (all distinct primes); size 64 is enough
       for all n that appear in the tests (n up to ~10^12). */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    /* factor_u64 collects distinct primes in ascending order. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* fewer than 2 distinct primes → denominator = 1 */
    if (cnt < 2)
        return (seq_t)1;

    /* rational sum as numerator/denominator */
    seq_t num = (seq_t)0;
    seq_t den = (seq_t)1;

    for (size_t i = 0; i + 1 < cnt; ++i) {
        seq_t a_num = (seq_t)primes[i];
        seq_t b_den = (seq_t)primes[i + 1];

        seq_t new_num = num * b_den + a_num * den;
        seq_t new_den = den * b_den;

        seq_t g = gcd(new_num, new_den);
        new_num /= g;
        new_den /= g;

        num = new_num;
        den = new_den;
    }
    return den;
}
