/*
 * A351097  Difference between {the maximal digit value in primorial base
 *          expansion of the arithmetic derivative of n} and {the maximal
 *          exponent in the prime factorization of n}.
 * https://oeis.org/A351097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9986, i16 n <= 9986, i32 n <= 9986, i64 n <= 9986,
 *            i128 n <= 9986, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A351097
    a(n) = A328390(n) - A051903(n)
    = (maximum digit in the primorial‑base expansion of the arithmetic
      derivative of n)  –  (maximum exponent in the prime factorisation of n)
    This implementation uses trial division with a small prime table
    and the greedy algorithm for primorial base conversion.  It is
    O(π(√n)) per call and works for n up to about 10000 in far
    less than 2 s. */

#include <stddef.h>
#include <stdint.h>

/* first 20 primes – sufficient for n ≤ 10^4 */
static const uint64_t PRIMES[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL
};
static const size_t NUM_PRIMES = sizeof(PRIMES) / sizeof(PRIMES[0]);

/* compute the arithmetic derivative of n
   and simultaneously find the maximal exponent in its factorisation. */
static void arith_derivative_and_maxexp(uint64_t n, uint64_t *deriv, unsigned *max_exp)
{
    uint64_t d = 0;
    unsigned max_e = 0;
    uint64_t temp = n;

    for (size_t i = 0; i < NUM_PRIMES && PRIMES[i] * PRIMES[i] <= temp; ++i) {
        uint64_t p = PRIMES[i];
        unsigned e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }
        if (e) {
            if (e > max_e) max_e = e;
            d += (uint64_t)e * (n / p);
        }
    }
    if (temp > 1) {        /* remaining prime factor */
        if (1 > max_e) max_e = 1;
        d += n / temp;
    }
    *deriv = d;
    *max_exp = max_e;
}

/* maximum digit in the primorial‑base expansion of x.
   weights[i] = product of the first i primes (weights[0] = 1). */
static unsigned max_digit_primorial_base(uint64_t x)
{
    /* store the weights that are ≤ x */
    uint64_t weights[32];
    size_t wcnt = 0;
    weights[wcnt++] = 1ULL;          /* weight[0] */

    for (size_t i = 0; i < NUM_PRIMES && wcnt < sizeof(weights)/sizeof(weights[0]); ++i) {
        uint64_t next = weights[wcnt - 1] * PRIMES[i];
        if (next > x) break;
        weights[wcnt++] = next;
    }

    unsigned max_digit = 0;
    uint64_t rem = x;
    /* greedy extraction from most significant weight */
    for (int i = (int)wcnt - 1; i >= 0; --i) {
        uint64_t w = weights[i];
        unsigned digit = (unsigned)(rem / w);  /* digit < PRIMES[i] by construction */
        if (digit > max_digit) max_digit = digit;
        rem -= (uint64_t)digit * w;
    }
    return max_digit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range, return 0 */

    uint64_t n_u = (uint64_t)n;
    uint64_t deriv;
    unsigned max_exp;
    arith_derivative_and_maxexp(n_u, &deriv, &max_exp);

    unsigned max_digit = max_digit_primorial_base(deriv);

    return (seq_t)max_digit - (seq_t)max_exp;
}
