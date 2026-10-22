/*
 * A326059  a(n) = A285309(n) - n, where A285309 gives the sum of nonsquare
 *          divisors of n.
 * https://oeis.org/A326059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A285309(n) - n
 * Range:     i8 n <= 83, i16 n <= 9856, i32 n <= 9856, i64 n <= 9856,
 *            i128 n <= 9856, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by enumerating the divisors of n, summing those that are not perfect squares,
   and then subtracting n. */
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* safety for unexpected zero index */

    /* factor n using the utility function */
    const size_t MAX_FACTORS = 32;   /* plenty for n <= 10^4 */
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* generate all divisors */
    seq_t divs[4096];                 /* generous capacity */
    size_t sz = 1;
    divs[0] = (seq_t)1;
    for (size_t fi = 0; fi < cnt; ++fi) {
        uint64_t p = primes[fi];
        uint64_t e = exps[fi];
        size_t old_sz = sz;
        seq_t power = (seq_t)1;
        for (uint64_t k = 1; k <= e; ++k) {
            power *= (seq_t)p;
            for (size_t j = 0; j < old_sz; ++j) {
                divs[sz++] = divs[j] * power;
            }
        }
    }

    /* sum the nonsquare divisors */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < sz; ++i) {
        uint64_t d = (uint64_t)divs[i];
        int is_square = 0;
        for (uint64_t t = 0; t * t <= d; ++t) {
            if (t * t == d) {
                is_square = 1;
                break;
            }
        }
        if (!is_square) {
            sum += (seq_t)d;
        }
    }

    return sum - (seq_t)n;
}
