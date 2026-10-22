/*
 * A357833  a(n) = Sum_{k=0..floor((n-2)/3)} 2^k * |Stirling1(n,3*k+2)|.
 * https://oeis.org/A357833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor((n-2)/3)} 2^k * |Stirling1(n,3*k+2)|
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b) */
#include "../utils/pow2.h"      /* pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* static caches for Stirling rows and sequence values */
    static seq_t *prev = NULL;          /* stores row for current cached_up_to */
    static seq_t *curr = NULL;          /* temporary row */
    static size_t prev_cap = 0, curr_cap = 0;

    static seq_t *a_cache = NULL;       /* a(n) cache */
    static size_t cache_cap = 0;
    static idx_t cached_up_to = 0;      /* largest n already computed */

    /* ---------- initialise on first call ---------- */
    if (prev == NULL) {
        prev_cap = 1;
        prev = (seq_t *)malloc(prev_cap * sizeof(seq_t));
        prev[0] = (seq_t)1;             /* S1(0,0)=1 */
    }
    if (curr == NULL) {
        curr_cap = 1;
        curr = (seq_t *)malloc(curr_cap * sizeof(seq_t));
    }
    if (a_cache == NULL) {
        cache_cap = 1;
        a_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)0;          /* a(0) = 0 */
    }

    /* ---------- ensure cache size for n ---------- */
    if (n >= cache_cap) {
        size_t old = cache_cap;
        cache_cap = (size_t)n + 1;
        a_cache = (seq_t *)realloc(a_cache, cache_cap * sizeof(seq_t));
        memset(a_cache + old, 0, (cache_cap - old) * sizeof(seq_t));
        a_cache[0] = (seq_t)0;
    }

    /* ---------- cached result ready? ---------- */
    if (n <= cached_up_to) {
        return a_cache[n];
    }

    /* ---------- extend Stirling rows up to n ---------- */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        /* extend storage if necessary */
        if ((size_t)i + 1 > prev_cap) {
            size_t old = prev_cap;
            prev_cap = (size_t)i + 1;
            prev = (seq_t *)realloc(prev, prev_cap * sizeof(seq_t));
            memset(prev + old, 0, (prev_cap - old) * sizeof(seq_t));
        }
        if ((size_t)i + 1 > curr_cap) {
            size_t old = curr_cap;
            curr_cap = (size_t)i + 1;
            curr = (seq_t *)realloc(curr, curr_cap * sizeof(seq_t));
        }

        /* ensure prev[i] = 0 for the recurrence (k=i term) */
        if ((size_t)i < prev_cap) {
            prev[i] = (seq_t)0;
        }

        /* compute curr row from prev row */
        curr[0] = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t t1 = prev[k - 1];
            seq_t t2 = seq_mul(prev[k], (seq_t)(i - 1));
            curr[k] = t1 + t2;
        }

        /* compute a(i) = sum_{j=2,5,8,.. <= i} 2^((j-2)/3) * curr[j] */
        seq_t sum = (seq_t)0;
        for (idx_t j = 2; j <= i; j += 3) {
            seq_t p2 = pow2((j - 2) / 3);
            seq_t prod = seq_mul(p2, curr[j]);
            sum = sum + prod;
        }
        a_cache[i] = sum;

        /* prepare for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    cached_up_to = n;
    return a_cache[n];
}
