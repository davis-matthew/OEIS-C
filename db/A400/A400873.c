/*
 * A400873  The total sum of exponential divisors of the exponential divisors
 *          of n.
 * https://oeis.org/A400873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, new
 * Formula:   a(n) = Sum_{d exponential divisor of n} A051377(d) A051377(p^e)
 *            = Sum_{f|e} p^f. The sequence is multiplicative. For a prime
 *            power p^e a(p^e) = Sum_{d|e} tau(d) * p^(e/d), where tau(d) =
 *            number of divisors of d
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq, tau_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"
#include "../utils/tau_int.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    /* Factorize n */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        int e = (int)exps[i];
        seq_t local = (seq_t)0;

        for (int d = 1; d <= e; ++d) {
            if (e % d != 0) continue;
            unsigned int td = tau_int(d);
            seq_t power = pow_seq((seq_t)primes[i], (int)(e / d));
            local += (seq_t)td * power;
        }
        result *= local;
    }
    return result;
}
