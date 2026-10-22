/*
 * A357984  Replace prime(k) with A000720(k) in the prime factorization of n.
 * https://oeis.org/A357984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} π(π(index of p))^e where index of the prime p
 *            is π(p) and π(k) is the prime-counting function
 * Range:     u8 n <= 5896, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 1;  /* a(1)=1 */

    uint64_t nn = (uint64_t)n;
    /* Prime factorisation of nn: primes[i]^exps[i], i=0..cnt-1 */
    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        size_t e = exps[i];

        /* pi(p) = index of prime p */
        seq_t pi_p = compute_pi((idx_t)p);
        /* pi(pi(p)) = replacement value for one occurrence of p */
        seq_t rep = compute_pi((idx_t)pi_p);

        /* contribution of p^e */
        seq_t contrib = seq_pow(rep, (idx_t)e);
        result *= contrib;
        if (result == 0) return 0;
    }
    return result;
}
