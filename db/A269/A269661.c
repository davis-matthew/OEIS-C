/*
 * A269661  a(n) = Product_{i=1..n} (5^i - 4^i).
 * https://oeis.org/A269661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..n} (5^i - 4^i)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq_array.h"

static seq_t current_pow5 = 1;
static seq_t current_pow4 = 1;
static seq_t current_prod = 1;
static idx_t current_index = 0;
static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        grow_seq_array(&cache, &cache_cap, new_cap);
    }

    if (n <= current_index) {
        return cache[(size_t)n];
    }

    for (idx_t i = current_index + 1; i <= n; ++i) {
        current_pow5 *= (seq_t)5;
        current_pow4 *= (seq_t)4;
        seq_t term = current_pow5 - current_pow4;
        current_prod *= term;
        cache[(size_t)i] = current_prod;
    }

    current_index = n;
    return cache[(size_t)n];
}
