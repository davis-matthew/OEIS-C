/*
 * A383383  Expansion of e.g.f. exp(-4*x) / (1-x)^5.
 * https://oeis.org/A383383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 1; a(n) = n*a(n-1) + 4*(n-1)*a(n-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t seq_cache_cap = 0;
static idx_t seq_cache_size = 0;

seq_t a(idx_t n)
{
    if (n >= seq_cache_size) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cache = tmp;
        seq_cache_cap = new_cap;
        for (idx_t i = seq_cache_size; i <= n; ++i) {
            if (i == 0 || i == 1) {
                seq_cache[i] = 1;
            } else {
                seq_t term1 = (seq_t)i * seq_cache[i-1];
                seq_t term2 = ((seq_t)4 * (seq_t)(i-1)) * seq_cache[i-2];
                seq_cache[i] = term1 + term2;
            }
        }
        seq_cache_size = n + 1;
    }
    return seq_cache[n];
}
