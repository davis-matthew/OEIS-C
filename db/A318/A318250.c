/*
 * A318250  a(n) = (n - 1)! * sigma_2(n), where sigma_2(n) = sum of squares of
 *          divisors of n (A001157).
 * https://oeis.org/A318250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n - 1)! * sigma_2(n) (sigma_2(n) = sum of squares of
 *            divisors of n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include <stdint.h>          /* for uint64_t */

static /* Helper to compute (base^exp) as seq_t */
seq_t pow_seq_uint64(uint64_t base, uint64_t exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    for (uint64_t i = 0; i < exp; ++i)
        result *= b;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* safety for out‑of‑range calls */
        return 0;

    /* Compute sigma_2(n) via prime factorisation */
    seq_t sigma = 1;

    uint64_t temp = (uint64_t)n;
    uint64_t primes[16];
    int exps[16];
    int count = 0;

    /* Count factors of 2 */
    int e = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e > 0) {
        primes[count] = 2;
        exps[count] = e;
        ++count;
    }

    /* Count odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[count] = p;
            exps[count] = e;
            ++count;
        }
    }

    if (temp > 1) {          /* remaining prime factor */
        primes[count] = temp;
        exps[count] = 1;
        ++count;
    }

    for (int i = 0; i < count; ++i) {
        uint64_t p = primes[i];
        uint64_t exp = (uint64_t)exps[i];

        /* numerator = p^(2*(exp+1)) - 1 */
        uint64_t num_exp = 2ULL * (exp + 1ULL);
        seq_t numerator = pow_seq_uint64(p, num_exp);
        numerator -= 1;

        /* denominator = p^2 - 1 */
        seq_t denominator = pow_seq_uint64(p, 2);
        denominator -= 1;

        seq_t term = numerator / denominator;
        sigma *= term;
    }

    /* factorial part */
    seq_t fact = factorial((idx_t)(n - 1));

    return fact * sigma;
}
