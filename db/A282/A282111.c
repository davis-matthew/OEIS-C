/*
 * A282111  Numbers n with k digits in base x (MSD(n)=d_k, LSD(n)=d_1) such
 *          that, chosen one of their digits in position d_k < j < d_1, is
 *          Sum_{i=j+1..k}{(i-j)*d_i} = Sum_{i=1..j-1}{(j-i)*d_i}. Case x = 6.
 * https://oeis.org/A282111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 4801, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9849 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A282111 in base 6 by brute‑force
 * search: each candidate number is converted to base‑6, its digits
 * are examined for a position j (2 ≤ j ≤ k‑1) that satisfies the
 * weighted fulcrum equation.
 * The algorithm stores found terms in a cache to allow arbitrary
 * call order and returns the required element. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool is_fulcrum_uint64(uint64_t v)
{
    if (v == 0)
        return false;

    /* Extract base‑6 digits, LSD first. */
    unsigned char d[64];
    int k = 0;
    uint64_t tmp = v;
    while (tmp > 0) {
        d[k++] = tmp % 6;
        tmp /= 6;
    }

    /* Build prefix sums: P1[i] = Σ_{t=1..i} d_{t-1},
     *                     P2[i] = Σ_{t=1..i} t*d_{t-1}. */
    uint64_t P1[65] = {0};
    uint64_t P2[65] = {0};
    for (int i = 1; i <= k; ++i) {
        P1[i] = P1[i-1] + d[i-1];
        P2[i] = P2[i-1] + (uint64_t)i * d[i-1];
    }

    /* Test all possible fulcrum positions j (2..k-1). */
    for (int j = 2; j <= k-1; ++j) {
        uint64_t left  = (uint64_t)j * P1[j-1] - P2[j-1];
        uint64_t right = (P2[k] - P2[j]) - (uint64_t)j * (P1[k] - P1[j]);
        if (left == right)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static seq_t next_start = 1;

    /* Initialise cache. */
    if (cache == NULL) {
        cache_cap = 32;
        cache = mem_new(seq_t, cache_cap);
        cache_sz = 0;
        next_start = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_sz < n) {
        seq_t val = next_start;
        /* Find the next qualifying number. */
        while (!is_fulcrum_uint64((uint64_t)val))
            val = val + 1;

        /* Store it. */
        if (cache_sz >= cache_cap) {
            cache_cap <<= 1;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_sz++] = val;

        /* Prepare for next search. */
        next_start = val + 1;
    }

    return cache[n-1];
}
