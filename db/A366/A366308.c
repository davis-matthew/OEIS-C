/*
 * A366308  The number of infinitary divisors of n that are terms of A366242.
 * https://oeis.org/A366308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_p 2^A139351(e), where e is the exponent of * each prime
 *            factor p of n and A139351(e) counts 1‑bits of e in even * binary
 *            positions (0‑based from the least significant bit)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, popcount_evenbits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/popcount_evenbits.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        unsigned k = popcount_evenbits(exps[i]);    /* A139351(e) */
        uint64_t pow2 = 1ULL << k;                  /* 2^k  (k ≤ 32) */
        res = res * (seq_t)pow2;                    /* multiply in seq_t */
    }
    return res;
}
