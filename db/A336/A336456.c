/*
 * A336456  a(n) = A335915(sigma(sigma(n))), where A335915 is a fully
 *          multiplicative sequence with a(2) = 1 and a(p) = A000265(p^2 - 1)
 *          for odd primes p, with A000265(k) giving the odd part of k.
 * https://oeis.org/A336456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A335915(sigma(sigma(n)))
 * Range:     u8 n <= 32, u16 n <= 1801, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, odd_part, pow_seq_large, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/odd_part.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    /* σ(n) */
    seq_t sigma1 = sigma_of(n);

    /* σ(σ(n)) */
    seq_t sigma2 = sigma_of((idx_t)sigma1);

    /* Factor σ(σ(n)) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)sigma2, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p64 = primes[i];
        /* a(2) = 1, ignore factor 2 */
        if (p64 == 2) {
            continue;
        }
        /* odd part of p^2 - 1 */
        uint64_t val = p64 * p64 - 1;
        seq_t base = odd_part((seq_t)val);
        seq_t exp  = (seq_t)exps[i];
        result *= pow_seq_large(base, exp);
    }
    return result;
}
