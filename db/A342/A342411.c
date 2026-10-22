/*
 * A342411  a(n) = Sum_{k=1..n} (n/gcd(k,n))^(n/gcd(k,n) - 2).
 * https://oeis.org/A342411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(d) * d^(d-2) (define 1^(-1)=1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, calc_phi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/calc_phi.h"

/* Recursively generate all divisors of n from its prime factorization
   and add each contribution phi(d)*d^(d-2) to the running total. */
static void
recurse_divisors(size_t idx, size_t num_factors,
                 uint64_t *pr, uint64_t *exps,
                 seq_t current, seq_t *sum)
{
    if (idx == num_factors) {
        seq_t d = current;
        seq_t term;
        if (d == 1) {
            /* phi(1)=1 and by convention 1^(-1)=1 */
            term = 1;
        } else {
            term = calc_phi(d);
            /* big_pow accepts a seq_t exponent; d-2 fits in seq_t for d>=2 */
            term = term * big_pow(d, d - 2);
        }
        *sum += term;
        return;
    }

    seq_t mult = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        recurse_divisors(idx + 1, num_factors,
                         pr, exps,
                         current * mult, sum);
        mult *= pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, guard */
    uint64_t N = (uint64_t)n;

    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);

    seq_t total = 0;
    recurse_divisors(0, cnt, primes, exps, (seq_t)1, &total);
    return total;
}
