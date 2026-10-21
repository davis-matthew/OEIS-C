/*
 * A245550  a(0)=0; for n >= 1, a(n) = f(n) - 2*f(floor((n-1)/2)), where f(n)
 *          = A006046(n).
 * https://oeis.org/A245550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = f(n) – 2*f(floor((n-1)/2))
   where f(0)=0, f(1)=1,
   f(2k)=3*f(k),
   f(2k+1)=f(2k)+2^popcount(k) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *f_cache = NULL;
static size_t f_cache_size = 0;

static void ensure_f_cache(idx_t up_to) {
    if (up_to < f_cache_size) return;
    size_t new_size = (size_t)up_to + 1;
    seq_t *tmp = seq_alloc_new_array(new_size);
    if (!tmp) abort();

    size_t old_size = f_cache_size;
    for (size_t i = 0; i < old_size; ++i) {
        tmp[i] = f_cache[i];
    }

    for (size_t i = old_size; i < new_size; ++i) {
        if (i == 0) {
            tmp[i] = (seq_t)0;
        } else if (i == 1) {
            tmp[i] = (seq_t)1;
        } else {
            if ((i & 1) == 0) {
                tmp[i] = ((seq_t)3) * tmp[i >> 1];
            } else {
                idx_t k = (i - 1) >> 1;
                seq_t pow2 = ((seq_t)1) << __builtin_popcountll(k);
                tmp[i] = tmp[i - 1] + pow2;
            }
        }
    }

    if (f_cache) seq_free_array(f_cache);
    f_cache = tmp;
    f_cache_size = new_size;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;
    ensure_f_cache(n);
    idx_t t = (n - 1) >> 1;
    return f_cache[(size_t)n] - ((seq_t)2 * f_cache[(size_t)t]);
}
