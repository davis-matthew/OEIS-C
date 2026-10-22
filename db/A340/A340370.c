/*
 * A340370  a(n) = 1 if the odd part of n is a squarefree semiprime, 0
 *          otherwise.
 * https://oeis.org/A340370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the odd part of n is a squarefree semiprime, 0
 *            otherwise
 * Range:     u8 n <= 9460, u16 n <= 9460, u32 n <= 9460, u64 n <= 9460,
 *            u128 n <= 9460, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed for OEIS offset 1, but handle zero safely */
    if (n == 0) return (seq_t)0;

    /* Compute odd part of n. */
    uint64_t odd = (uint64_t)n;
    while ((odd & 1ULL) == 0) odd >>= 1U;

    if (odd <= 1ULL) return (seq_t)0;      /* 1 or 0 cannot be a semiprime */

    /* Factor odd part using the provided helper. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(odd, primes, exps, &cnt);

    /* Must have exactly two distinct prime factors, each to the first power. */
    if (cnt != 2) return (seq_t)0;
    for (size_t i = 0; i < cnt; ++i)
        if (exps[i] != 1) return (seq_t)0;

    return (seq_t)1;
}
