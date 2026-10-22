/*
 * A366789  Fully multiplicative with a(p) = oddpart(primepi(p)).
 * https://oeis.org/A366789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e||n} oddpart(pi(p))^e
 * Range:     u8 n <= 1620, u16 n <= 9413, u32 n <= 9413, u64 n <= 9413,
 *            u128 n <= 9413, big any n
 * Helpers:   compute_pi, factor_u64, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"
#include "../utils/odd_part.h"

/* Fully multiplicative with a(p) = oddpart(pi(p)) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    uint64_t m = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        int e = (int)exps[i];
        seq_t base = odd_part(compute_pi(p));
        for (int j = 0; j < e; ++j)
            result = result * base;
    }
    return result;
}
