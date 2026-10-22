/*
 * A394177  Odd k such that there are exactly 2 bases 1 <= b <= k-1 in which k
 *          is a strong pseudoprime.
 * https://oeis.org/A394177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th odd integer k>1 such that - k has at least one
 *            prime factor ≡ 3 (mod 4), and - for every prime factor p of k,
 *            gcd(p−1, k−1) is a power of 2. The function computes the
 *            sequence by testing successive odd integers and caching the
 *            results for fast random access
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, is_power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/gcd.h"
#include "../utils/is_power_of_two.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_sz = 0, cache_cap = 0;
    static idx_t next_candidate = 3;   /* first odd > 1 */
    static bool init = false;

    if (!init) {
        cache_cap = 64;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        init = true;
    }

    while (cache_sz < n) {
        bool ok = true;
        bool has_3mod4 = false;

        /* factor the candidate (unsigned 64-bit number) */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t p_cnt;
        factor_u64((uint64_t)next_candidate, primes, exps, &p_cnt);

        for (size_t i = 0; i < p_cnt && ok; ++i) {
            uint64_t p = primes[i];
            if ((p & 3ULL) == 3ULL)          /* p ≡ 3 (mod 4) */
                has_3mod4 = true;

            seq_t g = gcd((seq_t)(p - 1), (seq_t)(next_candidate - 1));
            if (!is_power_of_two(g))
                ok = false;
        }

        if (ok && has_3mod4) {
            if (cache_sz == cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_sz++] = (seq_t)next_candidate;
        }

        next_candidate += 2;   /* move to the next odd integer */
    }

    return cache[n - 1];
}
