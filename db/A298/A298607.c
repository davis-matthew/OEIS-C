/*
 * A298607  Powers of 2 with the digit '0' in their decimal expansion.
 * https://oeis.org/A298607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th power of 2 whose decimal representation contains
 *            the digit 0
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   digit_frequencies, mul_small, seq_alloc_new_array,
 *            seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: incremental generation with caching. */

#include <stddef.h>
#include "../utils/mul_small.h"
#include "../utils/digit_frequencies.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache_ptr = NULL;
static idx_t cache_cap = 0;
static idx_t cache_size = 0;
static idx_t last_exp = 0;
static seq_t current_value = (seq_t)1;

/* Return the n-th term of A298607. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */

    /* If we already have enough terms cached, return directly. */
    if (n <= cache_size) {
        return cache_ptr[n - 1];
    }

    /* Ensure the cache can hold at least n terms. */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < n) new_cap = n;
        seq_t *new_cache = seq_alloc_new_array(new_cap);
        for (idx_t i = 0; i < cache_size; ++i) {
            new_cache[i] = cache_ptr[i];
        }
        if (cache_ptr) seq_free_array(cache_ptr);
        cache_ptr = new_cache;
        cache_cap = new_cap;
    }

    unsigned int freq[10];
    for (idx_t exp = last_exp + 1; ; ++exp) {
        current_value = mul_small(current_value, 2);   /* current_value = 2^exp */

        digit_frequencies(current_value, freq);
        if (freq[0] > 0) {
            cache_ptr[cache_size] = current_value;
            ++cache_size;
            if (cache_size == n) {
                last_exp = exp;
                return cache_ptr[n - 1];
            }
        }
        last_exp = exp;
    }
}
