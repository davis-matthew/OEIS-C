/*
 * A304182  Number of primitive inequivalent mirror-symmetric sublattices of
 *          rectangular lattice of index n.
 * https://oeis.org/A304182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} f(p,e) where * f(2,1) = 3, f(2,e≥2) = 4, f(p>2,
 *            e) = 2 * This implements the multiplicative definition from OEIS
 *            A304182
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* Factorization of n. 64 entries are more than enough for any 64‑bit number. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 2) {
            /* 2^1 → 3, 2^e (e>1) → 4 */
            result *= (seq_t)(exps[i] == 1 ? 3 : 4);
        } else {
            /* any other prime power → 2 */
            result *= (seq_t)2;
        }
    }
    return result;
}
