/*
 * A336401  a(n) = a(n-1) concatenated with the smallest number k, such that
 *          a(n) is divisible by lcm(1..n).
 * https://oeis.org/A336401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   digit_len, gcd, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336401
 *
 * a(1) = 1
 * a(n) = concatenation of a(n-1) and the smallest non‑negative integer k
 *        such that a(n) is divisible by lcm(1,2,…,n)
 *
 * The function memoises previous terms and LCMs so that arbitrary order
 * of calls is efficient.  All intermediate arithmetic is performed in
 * seq_t (or cast to idx_t where required) and no floating point or
 * non‑standard library calls are used. */

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"        /* gcd(seq_t, seq_t)       */
#include "../utils/pow10.h"      /* pow10(idx_t) -> seq_t   */
#include "../utils/digit_len.h"  /* digit_len(seq_t) -> int */

seq_t a(idx_t n)
{
    /* Handle the case where the caller asks for index 0 – behaviour
     * below the offset is irrelevant, so return 0. */
    if (n == 0)
        return (seq_t)0;

    /* static memoisation */
    static seq_t *a_cache   = NULL;  /* a(i) values, i≥0 */
    static seq_t *lcm_cache = NULL;  /* LCM(1…i) values, i≥0 */
    static size_t cache_sz = 0;      /* number of allocated slots */

    /* initialise caches to store at least index 1 */
    if (cache_sz == 0) {
        cache_sz   = 2;            /* indices 0 and 1 */
        a_cache   = (seq_t*)calloc(cache_sz, sizeof(seq_t));
        lcm_cache = (seq_t*)calloc(cache_sz, sizeof(seq_t));
        a_cache[1]   = (seq_t)1;   /* a(1) = 1 */
        lcm_cache[1] = (seq_t)1;   /* lcm(1) = 1 */
    }

    /* resize caches if needed */
    if (n >= cache_sz) {
        size_t new_sz = (size_t)n + 1;
        a_cache   = (seq_t*)realloc(a_cache,   new_sz * sizeof(seq_t));
        lcm_cache = (seq_t*)realloc(lcm_cache, new_sz * sizeof(seq_t));

        /* compute new terms up to index n */
        for (size_t i = cache_sz; i <= (size_t)n; ++i) {
            if (i == 1) continue;   /* already initialised */

            /* LCM(1…i) = lcm( LCM(1…i-1), i ) */
            seq_t prev_lcm = lcm_cache[i-1];
            seq_t g        = gcd(prev_lcm, (seq_t)i);
            seq_t cur_lcm  = (prev_lcm / g) * (seq_t)i;
            lcm_cache[i]   = cur_lcm;

            /* find the least non‑negative k with
             * concat(a[i-1], k) ≡ 0 (mod cur_lcm) */
            seq_t prev_a = a_cache[i-1];
            idx_t  k     = 0;
            seq_t k_seq, pow10_k, cand;
            bool  found  = false;

            while (!found) {
                k_seq    = (seq_t)k;
                pow10_k  = pow10((idx_t)digit_len(k_seq));
                cand     = prev_a * pow10_k + k_seq;
                if ((cand % cur_lcm) == (seq_t)0) {
                    found = true;
                } else {
                    ++k;
                }
            }

            /* store the computed a[i] */
            a_cache[i] = prev_a * pow10((idx_t)digit_len(k_seq)) + k_seq;
        }

        cache_sz = new_sz;
    }

    return a_cache[n];
}
