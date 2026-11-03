/*
 * A384915  The number of unordered factorizations of n into powers of primes
 *          of the form p^e where p is prime and 0 <= e <= p (A074583).
 * https://oeis.org/A384915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e∥n} A026820(e,p), where A026820(e,p) is the number
 *            of partitions of e whose parts are all ≤ p (i.e. partition
 *            number with maximum part size p). The sequence is multiplicative
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9405 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Return the number of partitions of e into parts ≤ p.
   Computed on the fly to avoid pre‑computation tables that may not fit
   into seq_t for small builds. */
static inline seq_t part_leq(uint64_t e, uint64_t p)
{
    if (e == 0)
        return 1;
    if (p == 0)
        return 0;

    size_t E = (size_t)e;
    size_t P = (size_t)p;
    if (P > E)
        P = E;

    seq_t dp[65];
    /* initialize array */
    for (size_t i = 0; i <= E; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (size_t j = 1; j <= P; ++j) {
        for (size_t k = j; k <= E; ++k) {
            dp[k] = dp[k] + dp[k - j];
        }
    }
    return dp[E];
}

/* Main OEIS sequence function. */
extern seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    uint64_t N = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(N, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        result *= part_leq(e, p);
    }
    return result;
}
