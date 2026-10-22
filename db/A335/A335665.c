/*
 * A335665  Product of the refactorable divisors of n.
 * https://oeis.org/A335665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{d|n, d is refactorable} d
 * Range:     u8 n <= 17, u16 n <= 35, u32 n <= 215, u64 n <= 719,
 *            u128 n <= 3599, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Recursively enumerate all divisors of n.
   primes   – distinct prime factors of n
   exps     – corresponding exponents
   idx      – current prime index
   total    – total number of distinct primes
   curr     – current divisor value being constructed
   tau      – number of divisors of the current divisor
   res      – pointer to the running product of refactorable divisors
*/
static void
gen_divisors(const uint64_t *primes,
             const uint64_t *exps,
             int idx,
             int total,
             seq_t curr,
             uint64_t tau,
             seq_t *res)
{
    if (idx == total) {
        /* Check if the constructed divisor is refactorable.
           d is refactorable if τ(d) divides d.  */
        if ((curr % (seq_t)tau) == (seq_t)0) {
            *res *= curr;
        }
        return;
    }

    seq_t pow = 1; /* primes[idx]^e */
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        uint64_t next_tau = tau * (e + 1);
        seq_t next_val = curr * pow;
        gen_divisors(primes, exps, idx + 1, total,
                     next_val, next_tau, res);
        pow *= (seq_t)primes[idx];
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* A335665 is defined for n ≥ 1 */

    /* Factor n into primes and exponents.  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t acc = 1;
    gen_divisors(primes, exps, 0, (int)cnt, (seq_t)1, 1, &acc);
    return acc;
}
