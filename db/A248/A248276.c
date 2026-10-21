/*
 * A248276  Egyptian fraction representation of sqrt(51) (A010504) using a
 *          greedy function.
 * https://oeis.org/A248276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Egyptian fraction representation of sqrt(51) by the greedy
 *            algorithm. a(0) = 7, a(1) = 8, a(2) = 61, a(3) = 28583,
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

#define D 51ULL   /* the square‑free number under the root */

static inline bool check_denom(seq_t a, seq_t p, seq_t q)
{
    /*   return (p/q + 1/a) < sqrt(D)   */
    seq_t apq  = a * p + q;
    seq_t left = apq * apq;                            /* (ap+q)²   */
    seq_t right= (seq_t)D * a * a * q * q;              /* D a² q²   */
    return left < right;
}

static seq_t next_denom(seq_t p, seq_t q)
{
    /* Use the inequality
          (a p + q)² < D a² q²
       to find the smallest a ≥ 1 that satisfies it.           */
    seq_t q2 = q * q;
    seq_t t  = (seq_t)D * q2 - p * p;          /* > 0                */
    seq_t sq = (seq_t)D * q2;

    seq_t s = floor_sqrt(sq);                  /* floor( sqrt(D) * q ) */

    seq_t numerator = p * q + q * s;           /* lower bound numerator */
    seq_t a = numerator / t;
    if (numerator % t != 0) ++a;               /* ceil division        */

    /* Adjust in case the ceil bound is too small or big. */
    while (!check_denom(a, p, q)) ++a;
    while (a > 1 && check_denom(a - 1, p, q)) --a;
    return a;
}

seq_t a(idx_t n)
{
    /* Caching of computed terms for arbitrary order of calls. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    static seq_t p_state = 0;   /* numerator of current best rational < sqrt(51)  */
    static seq_t q_state = 1;   /* denominator of current best rational < sqrt(51) */

    /* Ensure that a(0) = 7 is cached. */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)7;
        cache_len = 1;
        p_state = (seq_t)7;
        q_state = (seq_t)1;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute further terms up to n if they have not been cached. */
    while (cache_len <= (size_t)n) {
        seq_t ak = next_denom(p_state, q_state);
        cache[cache_len] = ak;

        /* Update the rational approximation for the next step. */
        seq_t new_p = ak * p_state + q_state;
        seq_t new_q = ak * q_state;
        p_state = new_p;
        q_state = new_q;

        ++cache_len;
    }

    return cache[(size_t)n];
}
