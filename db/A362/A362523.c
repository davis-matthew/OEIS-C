/*
 * A362523  a(n) = n! * Sum_{k=0..floor(n/3)} (k+1)^(k-1) / (k! * (n-3*k)!).
 * https://oeis.org/A362523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Σ_{k=0..⌊n/3⌋} (k+1)^(k-1) / (k! (n-3k)!)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* Static cache for factorials */
static seq_t *fact_cache = NULL;
static size_t fact_cache_len = 0;

/* Ensure factorial cache up to n (inclusive) is filled */
static void ensure_fact(idx_t n)
{
    if ((size_t)n + 1 > fact_cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(fact_cache, new_len * sizeof(seq_t));
        if (!new_cache) abort(); /* Out of memory */
        fact_cache = new_cache;
        /* Initialize if cache was empty */
        if (new_len > 0 && fact_cache_len == 0)
            fact_cache[0] = (seq_t)1;
        /* Compute new factorial values */
        for (size_t i = fact_cache_len; i < new_len; ++i) {
            if (i == 0)
                fact_cache[0] = (seq_t)1;
            else
                fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_cache_len = new_len;
    }
}

/* Main sequence generation function */
seq_t a(idx_t n)
{
    ensure_fact(n);
    seq_t n_fact = fact_cache[n];
    seq_t result = 0;

    for (idx_t k = 0; k <= n / 3; ++k) {
        /* Term: (n! / (k! * (n-3k)!)) * (k+1)^(k-1) */
        seq_t temp = n_fact / fact_cache[k];
        temp = temp / fact_cache[n - 3 * k];

        seq_t pow_term;
        if (k == 0)
            pow_term = (seq_t)1;
        else
            pow_term = big_pow((seq_t)(k + 1), (seq_t)(k - 1));

        temp = temp * pow_term;
        result += temp;
    }

    return result;
}
