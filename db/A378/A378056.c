/*
 * A378056  a(n) = gcd(A057643(n), A084190(n)) = gcd(lcm{d+1 : d|n}, lcm{d-1 :
 *          d > 1 and d|n}).
 * https://oeis.org/A378056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( lcm{d+1 : d|n}, lcm{d-1 : d>1 and d|n} )
 * Range:     u8 n <= 35, u16 n <= 119, u32 n <= 629, u64 n <= 3359,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_to_exp, factor_u64, gcd, seq_mul,
 *            seq_pow, update_max
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A378056. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/factor_to_exp.h"
#include "../utils/update_max.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n < 1 */

    /* ----- 1. Factorize n and obtain its prime factorization ----- */
    uint64_t pf_n[64];          /* prime factors of n  */
    uint64_t exp_n[64];         /* corresponding exponents   */
    size_t pcnt = 0;
    factor_u64((uint64_t)n, pf_n, exp_n, &pcnt);

    /* ----- 2. Generate all divisors of n ----- */
    /* maximum number of divisors for n<=10000 is < 2000, use 4096 buffer */
    const size_t MAXD = 4096;
    uint64_t divisors[MAXD];
    size_t dsz = 1;
    divisors[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pf_n[i];
        unsigned e = (unsigned)exp_n[i];
        size_t old = dsz;
        for (size_t j = 0; j < old; ++j) {
            uint64_t base = divisors[j];
            uint64_t mul  = 1;
            for (unsigned k = 1; k <= e; ++k) {
                mul *= p;
                divisors[dsz++] = base * mul;
            }
        }
    }

    /* ----- 3. Prepare primes up to n+1 for factorization of d±1 ----- */
    uint64_t *prime_list = NULL;
    size_t primecnt = 0;
    build_primes_upto((idx_t)n + 1, &prime_list, &primecnt);

    /* arrays holding maximum exponents for l1 and l2 */
    uint8_t exp1[primecnt];
    uint8_t exp2[primecnt];
    for (size_t i = 0; i < primecnt; ++i) {
        exp1[i] = 0;
        exp2[i] = 0;
    }

    /* ----- 4. Factor each d+1 and d-1, updating exponent arrays ----- */
    uint8_t local[primecnt];
    for (size_t i = 0; i < dsz; ++i) {
        uint64_t d = divisors[i];

        /* factor d+1 */
        for (size_t k = 0; k < primecnt; ++k) local[k] = 0;
        factor_to_exp(d + 1, prime_list, primecnt, local);
        update_max(exp1, local, primecnt);

        /* factor d-1 if d>1 */
        if (d > 1) {
            for (size_t k = 0; k < primecnt; ++k) local[k] = 0;
            factor_to_exp(d - 1, prime_list, primecnt, local);
            update_max(exp2, local, primecnt);
        }
    }

    /* ----- 5. Compute the gcd of the two lcms: product of primes^min(exp1,exp2) ----- */
    seq_t result = 1;
    for (size_t i = 0; i < primecnt; ++i) {
        uint8_t common = exp1[i] < exp2[i] ? exp1[i] : exp2[i];
        if (common > 0) {
            seq_t p_pow = seq_pow((seq_t)prime_list[i], (idx_t)common);
            result = seq_mul(result, p_pow);
        }
    }

    free(prime_list);
    return result;
}
