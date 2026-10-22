/*
 * A363877  Number of divisors of 7*n-2 of form 7*k+3.
 * https://oeis.org/A363877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of divisors of 7*n-2 that are congruent to 3 (mod 7).  
   a(n) = #{ d | d | (7*n-2)  and  d ≡ 3 (mod 7) } */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/factor_u64.h"   /* provides factor_u64(...) */

seq_t
a(idx_t n)
{
    /* compute m = 7*n-2 */
    uint64_t m = (uint64_t)n * 7ULL - 2ULL;

    /* factorise m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  pcnt = 0;
    factor_u64(m, primes, exps, &pcnt);

    /* dynamic programming over residues modulo 7 */
    unsigned long long ways[7] = {0};
    ways[1 % 7] = 1;                     /* divisor 1 contributes residue 1 */

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p   = primes[i];
        int      e   = (int)exps[i];

        /* pre‑compute p^k mod 7 for 0 ≤ k ≤ e */
        int pkmod[32];
        pkmod[0] = 1 % 7;
        int pmod = (int)(p % 7);
        for (int k = 1; k <= e; ++k)
            pkmod[k] = (pkmod[k - 1] * pmod) % 7;

        unsigned long long newways[7] = {0};

        for (int r = 0; r < 7; ++r) {
            if (ways[r] == 0) continue;
            for (int k = 0; k <= e; ++k) {
                int nr = (r * pkmod[k]) % 7;
                newways[nr] += ways[r];
            }
        }
        memcpy(ways, newways, sizeof(ways));
    }

    /* return number of divisors with residue 3 (mod 7) */
    return (seq_t)ways[3];
}
