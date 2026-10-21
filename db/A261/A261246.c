/*
 * A261246  Positive integers D such that the generalized Pell equation X^2 -
 *          D Y^2 = 2 is soluble.
 * https://oeis.org/A261246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 53, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computation:  For each D, test whether the generalized Pell equation
 * X² – D Y² = 2 has an integer solution.  The test uses the continued‑
 * fraction expansion of √D; a solution exists iff it appears among the
 * convergents before the period repeats.  The sequence a(n) lists the
 * nth D (1‑based) for which this is true, non‑squares only.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/seq_realloc.h"

/* Return true if the equation X^2 – D Y^2 = 2 has a solution. */
static bool has_solution(seq_t D)
{
    /* D = 2 is a trivial solution (X=2,Y=1).  */
    if (D == 2)
        return true;

    /* Exclude perfect squares. */
    seq_t sqrtD = floor_sqrt(D);
    if (sqrtD * sqrtD == D)
        return false;

    /* Convert D to 64‑bit unsigned for internal arithmetic. */
    uint64_t Dval = (uint64_t)D;

    /* Continued–fraction parameters (all 128‑bit to avoid overflow). */
    unsigned __int128 m = 0;
    unsigned __int128 d = 1;
    unsigned __int128 a = (unsigned __int128)(uint64_t)sqrtD;
    unsigned int a0 = (uint32_t)(uint64_t)sqrtD;

    /* Convergents: p_{-1}=1, p_0=a0; q_{-1}=0, q_0=1. */
    __int128 p_prev = 1;
    __int128 p      = a;
    __int128 q_prev = 0;
    __int128 q      = 1;

    /* Check the first convergent. */
    __int128 diff = p * p - (__int128)Dval * q * q;
    if (diff == 2)
        return true;

    /* Iterate through the period of the continued fraction. */
    while (true) {
        /* Next terms of the continued fraction. */
        m = d * a - m;
        d = (unsigned __int128)(Dval - (uint64_t)m * (uint64_t)m) / d;
        a = ((unsigned __int128)a0 + m) / d;

        /* Update convergents: p = a·p + p_prev, q = a·q + q_prev. */
        __int128 p_next = (__int128)a * p + p_prev;
        __int128 q_next = (__int128)a * q + q_prev;
        p_prev = p;
        p = p_next;
        q_prev = q;
        q = q_next;

        /* Check this convergent. */
        diff = p * p - (__int128)Dval * q * q;
        if (diff == 2)
            return true;

        /* The period ends when a == 2*a0.  */
        if (a == (unsigned __int128)(2 * a0))
            break;
    }
    return false;
}

/* Return the nth term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;   /* Dynamic array of found D’s.    */
    static size_t cache_len = 0;  /* Number of terms stored.        */
    static size_t cache_cap = 0;  /* Current allocation capacity.   */
    static seq_t next_D = 2;      /* Next integer to test for D.    */

    /* Build the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Find the next D with a solution.  */
        while (!has_solution(next_D))
            ++next_D;

        /* Grow the cache if necessary.  */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            cache = seq_realloc(cache, cache_cap, new_cap);
            cache_cap = new_cap;
        }

        /* Store the found D.  */
        cache[cache_len++] = next_D;
        ++next_D;
    }

    return cache[n - 1];
}
