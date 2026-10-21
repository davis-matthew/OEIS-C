/*
 * A286324  a(n) is the number of bi-unitary divisors of n.
 * https://oeis.org/A286324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (e + (e mod 2)) (product over prime powers
 *            dividing n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   /* declares factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1; n = 0 is unused by the OEIS offset */
        return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64], exps[64];  /* safe for all 64‑bit integers */
    size_t cnt = 0;

    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        seq_t term = (seq_t)(e + (e & 1ULL));  /* e + (e mod 2) */
        result *= term;
    }
    return result;
}
