/*
 * A353351  Number of divisors d of n for which A048675(d) is not a multiple
 *          of 3.
 * https://oeis.org/A353351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} [A048675(d) mod 3 != 0], with A048675(p_i^e) =
 *            e*2^(i-1); counted by a DP over the residues mod 3 across the
 *            prime-power factors of n
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9330 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* number of primes <= p (simple sieve) */
static uint64_t A353351_prime_count(uint64_t p)
{
    unsigned char *comp;
    uint64_t i, j, c = 0;
    if (p < 2) return 0;
    comp = (unsigned char *)calloc((size_t)(p + 1), 1);
    if (!comp) return 0;
    for (i = 2; i <= p; ++i) {
        if (!comp[i]) {
            ++c;
            if (i <= p / i)
                for (j = i * i; j <= p; j += i) comp[j] = 1;
        }
    }
    free(comp);
    return c;
}

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t dp[3] = {1, 0, 0};
    size_t i;

    if (n == 0) return (seq_t)0;
    if (n > 1) factor_u64((uint64_t)n, primes, exps, &cnt);

    for (i = 0; i < cnt; ++i) {
        uint64_t pi = A353351_prime_count(primes[i]);
        /* 2^(pi-1) mod 3 is 1 when pi is odd, 2 when pi is even */
        unsigned w = (pi & 1) ? 1u : 2u;
        uint64_t e = exps[i];
        uint64_t nd[3] = {0, 0, 0};
        uint64_t k;
        for (k = 0; k <= e; ++k) {
            unsigned shift = (unsigned)((k % 3) * w % 3);
            unsigned r;
            for (r = 0; r < 3; ++r)
                nd[(r + shift) % 3] += dp[r];
        }
        dp[0] = nd[0]; dp[1] = nd[1]; dp[2] = nd[2];
    }
    return (seq_t)(dp[1] + dp[2]);
}
