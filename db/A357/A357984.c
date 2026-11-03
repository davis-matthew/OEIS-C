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
 * Validated: standard, pass, 103 terms through n = 9181 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"
#include "../utils/prime_pi.h"

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
        seq_t pi_p = prime_pi((idx_t)p);
        /* pi(pi(p)) = replacement value for one occurrence of p */
        seq_t rep = prime_pi((idx_t)pi_p);

        /* contribution of p^e */
        seq_t contrib = ipow_idx(rep, (idx_t)e);
        result *= contrib;
        if (result == 0) return 0;
    }
    return result;
}
