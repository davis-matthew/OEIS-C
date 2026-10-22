/*
 * A353678  a(n) = 1 if the ratio of Euler totient phi to Carmichael's lambda
 *          function is 2, otherwise 0.
 * https://oeis.org/A353678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2512, u16 n <= 8862, u32 n <= 8862, u64 n <= 8862,
 *            u128 n <= 8862, big any n
 * Helpers:   calc_phi, factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A353678
 *
 *  Formula: a(n) = 1 if φ(n) / λ(n) = 2, otherwise 0
 *
 *  Defines an external function
 *      seq_t a(idx_t n)
 *
 *  OEIS offset: 1
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"
#include "../utils/calc_phi.h"

/* Compute Carmichael's function λ(n) for n > 0 */
static seq_t lambda_of(uint64_t n)
{
    if (n <= 1)
        return 1;            /* λ(1) = 1 */

    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t l = 1;            /* current lcm initialized to 1 */

    for (size_t i = 0; i < cnt; ++i) {
        seq_t comp;

        if (primes[i] == 2ULL) {          /* special handling for 2^k */
            if (exps[i] == 1ULL)
                comp = 1;
            else if (exps[i] == 2ULL)
                comp = 2;
            else
                comp = ((seq_t)1 << (exps[i] - 2));   /* 2^{k-2} */
        } else {                           /* odd prime powers */
            /* λ(p^e) = p^{e-1} * (p-1)  for odd p */
            seq_t powp = pow_uint(primes[i], exps[i] - 1);
            comp = powp * ((seq_t)primes[i] - 1);
        }

        seq_t g = gcd(l, comp);            /* g = gcd(l, comp) */
        l = (l / g) * comp;                /* l = lcm(l, comp) */
    }

    return l;
}

/*  External function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* sequence is defined for n ≥ 1 */

    seq_t phi = calc_phi((seq_t)n);          /* Euler's totient */
    seq_t lam = lambda_of((uint64_t)n);      /* Carmichael's lambda */

    return (phi == lam * 2) ? 1 : 0;
}
