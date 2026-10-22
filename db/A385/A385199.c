/*
 * A385199  The number of integers k from 1 to n such that the greatest
 *          divisor of k that is either 1 or a prime power (A000961).
 * https://oeis.org/A385199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = # { 1 ≤ k ≤ n | the largest unitary divisor of n that divides k is either 1 or a prime power } 
 * Computed naively in O(n·r) where r = number of distinct prime powers of n.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* not within OEIS offset */

    /* factorize n → prime powers p_i^e_i */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* precompute the prime powers of n */
    uint64_t pw[64];
    size_t r = cnt;
    for (size_t i = 0; i < r; ++i) {
        uint64_t p = primes[i];
        uint64_t val = 1;
        for (size_t e = 0; e < exps[i]; ++e) val *= p;
        pw[i] = val;
    }

    seq_t ans = 0;

    for (idx_t k = 1; k <= n; ++k) {
        size_t divCnt = 0;
        for (size_t i = 0; i < r; ++i) {
            if ((uint64_t)k % pw[i] == 0) {
                ++divCnt;
                if (divCnt > 1) break;   /* more than one prime‑power divisor → reject */
            }
        }
        if (divCnt <= 1) ++ans;
    }

    return ans;
}
