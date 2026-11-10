/*
 * A261247  Positive fundamental solution x(n) of the generalized Pell
 *          equation X^2 - D(n) Y(n) = 2 with D(n) = A261246(n).
 * https://oeis.org/A261247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest x > 0 with x^2 - D*y^2 = 2, D = A261246(n);
 *            found as the first convergent p_k/q_k of the continued fraction
 *            of sqrt(D) with p_k^2 - D q_k^2 = (-1)^(k+1) Q_(k+1) = 2 (D = 2
 *            handled directly: x = 2)
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   ipow_i32, isqrt_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/isqrt_u64.h"
#include "../utils/mem.h"

/* ---- A261246 (copied, renamed) ---- */

static inline seq_t A261246_floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static bool A261246_has_solution(seq_t D)
{
    if (D == 2)
        return true;

    seq_t sqrtD = A261246_floor_sqrt(D);
    if (sqrtD * sqrtD == D)
        return false;

    uint64_t Dval = (uint64_t)D;

    unsigned __int128 m = 0;
    unsigned __int128 d = 1;
    unsigned __int128 a = (unsigned __int128)(uint64_t)sqrtD;
    unsigned int a0 = (uint32_t)(uint64_t)sqrtD;

    __int128 p_prev = 1;
    __int128 p      = a;
    __int128 q_prev = 0;
    __int128 q      = 1;

    __int128 diff = p * p - (__int128)Dval * q * q;
    if (diff == 2)
        return true;

    while (true) {
        m = d * a - m;
        d = (unsigned __int128)(Dval - (uint64_t)m * (uint64_t)m) / d;
        a = ((unsigned __int128)a0 + m) / d;

        __int128 p_next = (__int128)a * p + p_prev;
        __int128 q_next = (__int128)a * q + q_prev;
        p_prev = p;
        p = p_next;
        q_prev = q;
        q = q_next;

        diff = p * p - (__int128)Dval * q * q;
        if (diff == 2)
            return true;

        if (a == (unsigned __int128)(2 * a0))
            break;
    }
    return false;
}

static seq_t A261246_a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_D = 2;

    while (cache_len < n) {
        while (!A261246_has_solution(next_D))
            ++next_D;

        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (cache == NULL)
                cache = mem_new(seq_t, new_cap);
            else
                cache = mem_resize(seq_t, cache, new_cap);
            cache_cap = new_cap;
        }

        cache[cache_len++] = next_D;
        ++next_D;
    }

    return cache[n - 1];
}

/* ---- A261247 ---- */

seq_t a(idx_t n)
{
    seq_t Ds = A261246_a(n);
    uint64_t D = (uint64_t)Ds;
    if (D == 2)
        return (seq_t)2;

    uint64_t a0 = isqrt_u64(D);
    uint64_t m = 0, d = 1, ak = a0;

    seq_t p_prev = 1;
    seq_t p = (seq_t)a0;
    unsigned long long k = 0;
    /* p_k^2 - D q_k^2 = (-1)^(k+1) * Q_(k+1) */
    for (unsigned long long iter = 0; iter < 100000000ULL; ++iter) {
        m = d * ak - m;
        d = (D - m * m) / d;          /* d = Q_(k+1) */
        if (d == 2 && (k & 1ULL))
            return p;
        ak = (a0 + m) / d;
        seq_t p_next = (seq_t)ak * p + p_prev;
        p_prev = p;
        p = p_next;
        ++k;
    }
    return 0;
}
