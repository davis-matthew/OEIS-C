/*
 * A362748  E.g.f. satisfies A(x) = exp(x^3/6 + x * A(x)).
 * https://oeis.org/A362748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A362748
   Formula: a(n) = n! * Sum_{k=0..floor(n/3)} (n-3k+1)^(n-2k-1) / (6^k * k! * (n-3k)!)
   Offset: 0 (a(0)=1) 
   This implementation uses a dynamic cache so that any order of calls is
   supported.  It relies on the `big_pow` routine from utils/big_pow.h
   for arbitrary‑precision exponentiation.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* ensure cache is large enough */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort (not recoverable) */
            exit(EXIT_FAILURE);
        }
        /* initialise new entries to zero */
        for (size_t i = cache_size; i < new_size; ++i) {
            tmp[i] = 0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* a(0) is 1; guard against zero caching */
    if (n == 0) {
        cache[0] = (seq_t)1;
        return (seq_t)1;
    }

    /* return cached result if present */
    if (cache[(size_t)n] != (seq_t)0) {
        return cache[(size_t)n];
    }

    idx_t max_k = n / 3;          /* floor(n/3) */
    seq_t sum = 0;

    /* pre‑compute running factors for the ratio, k! and 6^k */
    seq_t ratio   = 1;    /* product of (n-3k+1)..n for current k */
    seq_t fact_k  = 1;    /* k! for current k */
    seq_t pow6    = 1;    /* 6^k for current k */

    for (idx_t k = 0; k <= max_k; ++k) {
        /* coefficient = ratio / (fact_k * pow6)  (exact division) */
        seq_t coeff = ratio;
        coeff /= fact_k;
        coeff /= pow6;

        /* base = n-3k+1 ; exponent = n-2k-1 */
        seq_t base = (seq_t)(n - 3 * k + 1);
        seq_t exp  = (seq_t)(n - 2 * k - 1);

        seq_t pow_term = big_pow(base, exp);
        seq_t term = coeff * pow_term;

        sum += term;

        /* prepare for next iteration (k+1) if not last */
        if (k < max_k) {
            idx_t nextk = k + 1;
            seq_t a1 = (seq_t)(n - 3 * nextk + 1);
            seq_t a2 = (seq_t)(n - 3 * nextk + 2);
            seq_t a3 = (seq_t)(n - 3 * nextk + 3);
            ratio = ratio * a1 * a2 * a3;
            fact_k = fact_k * (seq_t)nextk;
            pow6   = pow6 * (seq_t)6;
        }
    }

    cache[(size_t)n] = sum;
    return sum;
}
