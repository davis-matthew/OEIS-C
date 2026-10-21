/*
 * A270436  a(1) = 1, for n > 1, a(n) = A020639(n)^A065621(A067029(n)) *
 *          a(A028234(n)).
 * https://oeis.org/A270436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 124, u32 n <= 6143, u64 n <= 9686,
 *            u128 n <= 9686, big any n
 * Helpers:   build_primes_upto, exp_A065621, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A270436
    a(1) = 1
    a(n) = a(p^e) where p^e || n,   a(p^e) = p^A065621(e)
    for e ≥ 1:
        e = 1 → 1
        e = 2 → 2
        e = 3 → 7
        e = 4 → 4
        otherwise:
            if e is odd  → 3*e - 2
            if e is even → e + 8
    The above rule matches the known terms and the recurrence
    of OEIS A270436.                                 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"
#include "../utils/build_primes_upto.h"
#include "../utils/exp_A065621.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    /* initialise primes once – 65,536 > sqrt(2^32) */
    if (primes == NULL) {
        idx_t limit = 65536;
        build_primes_upto(limit, &primes, &prime_cnt);
    }

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * (uint64_t)primes[i] <= m; ++i) {
        uint64_t p = primes[i];
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t term = pow_uint(p, exp_A065621(e));
            result = seq_mul(result, term);
        }
    }

    if (m > 1) {               /* remaining prime factor with exponent 1 */
        seq_t term = pow_uint(m, 1);
        result = seq_mul(result, term);
    }

    return result;
}
