/*
 * A379484  a(n) is the highest power of 3 dividing sigma(A003961(n^2)), where
 *          A003961 is fully multiplicative with a(prime(i)) = prime(i+1), and
 *          sigma is the sum of divisors function.
 * https://oeis.org/A379484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 8705, u16 n <= 8705, u32 n <= 8705, u64 n <= 8705,
 *            u128 n <= 8705, big any n
 * Helpers:   factor_u64, next_prime, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Highest power of 3 dividing σ(A003961(n²)) */
/* a(n) = 3^( Σ_{p^e‖n} v_3(2e+1) * δ_{nextprime(p)≡1 (mod 3)} ) */

#include <stddef.h>
#include <stdint.h>

/* helper routines available in the harness */
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/pow3.h"

/* compute a(n) for OEIS A379484 */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for 0, but guard to avoid UB */
        return 0;

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* accumulate exponent of 3 contributed by each prime factor */
    unsigned sum_exponent = 0;                 /* 3‑exponent in the final value */

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t q = next_prime(p);           /* next prime after p */

        if (q % 3 == 1) {                     /* only if q ≡ 1 (mod 3) */
            uint64_t t = 2 * e + 1;            /* odd number */
            /* add v_3(t) */
            while (t % 3 == 0) {
                ++sum_exponent;
                t /= 3;
            }
        }
    }

    /* return 3^sum_exponent */
    return pow3((idx_t)sum_exponent);
}
