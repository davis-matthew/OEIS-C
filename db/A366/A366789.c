/*
 * A366789  Fully multiplicative with a(p) = oddpart(primepi(p)).
 * https://oeis.org/A366789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e||n} oddpart(pi(p))^e
 * Range:     u8 n <= 1620, u16 n <= 52711, u32 n <= 52711, u64 n <= 52711,
 *            u128 n <= 52711, big any n
 * Validated: standard, pass, 116 terms through n = 9413 (2026-11-01)
 * Helpers:   factor_u64, odd_part, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/odd_part.h"
#include "../utils/prime_pi.h"

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
        seq_t base = odd_part(prime_pi(p));
        for (int j = 0; j < e; ++j)
            result = result * base;
    }
    return result;
}
