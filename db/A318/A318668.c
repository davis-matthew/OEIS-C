/*
 * A318668  a(n) = gcd(n, A064988(n)).
 * https://oeis.org/A318668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A064988(n)), where A064988(n) is product of
 *            prime(p)^v_p(n) for all primes p|n. a(n) = product over primes
 *            p|n of prime(p)^(min(v_p(n), v_{prime(p)}(n)))
 * Range:     u8 n <= 1704, u16 n <= 9578, u32 n <= 9578, u64 n <= 9578,
 *            u128 n <= 9578, big any n
 * Helpers:   factor_u64, get_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t q_seq = get_prime(primes[i]);     /* prime(p) */
        /* find exponent of q_seq in n */
        size_t j = cnt;
        for (size_t k = 0; k < cnt; ++k) {
            if ((seq_t)primes[k] == q_seq) {
                j = k;
                break;
            }
        }
        if (j == cnt)   /* q not a divisor of n */
            continue;

        /* exponent to use: min(v_p, v_q) */
        uint64_t exp = exps[i] < exps[j] ? exps[i] : exps[j];
        for (uint64_t t = 0; t < exp; ++t) {
            result = seq_mul(result, q_seq);
        }
    }

    return result;
}
