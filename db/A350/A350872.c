/*
 * A350872  Number of coincidence site lattices of index n in square lattice.
 * https://oeis.org/A350872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over distinct primes p ≡ 1 (mod 4) of 2, 0 if n
 *            has any prime factor ≠ 1 mod 4 (including 2). a(1)=1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t num = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(num, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        if ((primes[i] & 3) != 1)   /* p % 4 != 1 */
            return 0;
        result *= (seq_t)2;         /* multiply by 2 for each distinct prime */
    }
    return result;
}
