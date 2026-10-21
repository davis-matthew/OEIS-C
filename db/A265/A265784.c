/*
 * A265784  Numerators of lower primes-only best approximates (POBAs) to
 *          sqrt(5); see Comments.
 * https://oeis.org/A265784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, more
 * Formula:   The numbers a(n) are the numerators p of the lower primes‑only
 *            best * approximates (POBAs) to sqrt(5). For each prime
 *            denominator q we take the * largest prime p ≤ floor( sqrt(5)*q
 *            ); the pair (p,q) is kept if p/q is * larger than all previously
 *            kept fractions
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   ensure_capacity, floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/floor_sqrt.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t prev_q = 0;
    static uint64_t prev_p = 0;  /* store previous numerator in 64‑bit */

    if (n == 0) return 0;              /* safety – the sequence starts at a(1) */

    if (cache_len >= n)
        return cache[n - 1];

    /* start search from the smallest prime denominator if nothing computed yet,
       otherwise from the successor of the last accepted denominator. */
    idx_t candidate = (cache_len == 0) ? 2 : prev_q + 1;

    while (cache_len < n) {
        /* find the next prime denominator q */
        while (!is_prime(candidate))
            ++candidate;
        idx_t q = candidate;

        /* compute m = floor( sqrt(5*q*q) ) and find the largest prime ≤ m */
        seq_t tmp = (seq_t)5 * (seq_t)q * (seq_t)q;
        seq_t m_seq = floor_sqrt(tmp);
        idx_t m = (idx_t)m_seq;        /* m is small enough for idx_t */

        uint64_t p = m;
        while (p > 1 && !is_prime((idx_t)p))
            --p;

        /* accept if first term or if p/q > previous best fraction */
        bool accept = false;
        if (cache_len == 0) {
            accept = true;
        } else {
            seq_t left  = (seq_t)p * (seq_t)prev_q;
            seq_t right = (seq_t)prev_p * (seq_t)q;
            if (left > right)
                accept = true;
        }

        if (accept) {
            ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache,
                            sizeof(seq_t));
            cache[cache_len++] = (seq_t)p;
            prev_q = q;
            prev_p = p;
        }

        /* move to next integer to look for the next prime denominator */
        ++candidate;
    }

    return cache[n - 1];
}
