/*
 * A341008  Numbers whose sum of even digits and sum of odd digits differ by
 *          7.
 * https://oeis.org/A341008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 3999, u32 n <= 5001, u64 n <= 5001,
 *            u128 n <= 5001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose sum of even digits and sum of odd digits differ by 7 */
#include <stdlib.h>

/* Utility: reallocate a seq_t array (provided by the utils) */
#define seq_alloc_new(cap)  ((seq_t*)malloc((cap)*sizeof(seq_t)))
#define seq_realloc(old, oldcap, newcap) ((seq_t*)realloc((old), (newcap)*sizeof(seq_t)))

/* Return the n-th integer whose sum of even digits and sum of odd digits
 * differ by 7 (|sum_even - sum_odd| = 7).  a(1) = 7.  Uses lazy
 * generation and caching so that calls in any order are efficient. */
seq_t a(idx_t n)
{
    if (n == 0u) return 0;          /* offset is 1, so n>=1 in normal use */

    /* static cache of found terms and state for incremental generation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_to_test = 1u;    /* smallest integer not yet examined */

    /* Ensure we have at least n terms computed */
    while (cache_len < (size_t)n) {
        seq_t val = next_to_test++;
        seq_t even_sum = 0u, odd_sum = 0u;
        seq_t temp = val;
        while (temp > 0u) {
            seq_t d = temp % 10u;
            if (d & 1u) odd_sum += d;
            else even_sum += d;
            temp /= 10u;
        }
        if (even_sum > odd_sum) {
            if (even_sum - odd_sum == 7u) {
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0u) ? 16u : cache_cap * 2u;
                    seq_t *newp = seq_realloc(cache, cache_cap, new_cap);
                    cache = newp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
            }
        } else {
            if (odd_sum - even_sum == 7u) {
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0u) ? 16u : cache_cap * 2u;
                    seq_t *newp = seq_realloc(cache, cache_cap, new_cap);
                    cache = newp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
            }
        }
    }

    return cache[n-1];
}
