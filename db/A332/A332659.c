/*
 * A332659  Alternate multiplying and adding prime numbers: a(2n) = a(2n-1) +
 *          prime(2n+1) and a(2n-1) = a(2n) * prime(2n-2) for n >= 1.
 * https://oeis.org/A332659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   get_prime, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A332659:
   a(1) = 2 * 3 = 6
   a(2n)   = a(2n-1) + prime(2n+1)
   a(2n-1) = a(2n-2) * prime(2n)   (for n ≥ 2) */
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache = NULL;          /* cached values of a(n) */
static idx_t cache_cap = 0;          /* allocated capacity of cache */
static idx_t computed = 0;           /* highest index already computed (0 if none) */

static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = seq_alloc_new_array(new_cap);
    if (!tmp) abort(); /* out‑of‑memory */
    for (idx_t i = 0; i < cache_cap; ++i) tmp[i] = cache[i];
    if (cache) seq_free_array(cache);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n<1, but keep safe */

    ensure_capacity(n);

    /* compute values sequentially from the first missing index */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            /* a(1) = prime(1) * prime(2) = 2 * 3 */
            val = (seq_t)2 * (seq_t)3;
        } else {
            uint32_t p = get_prime(i + 1); /* prime(i+1) is used for both cases */
            if ((i & 1) == 0) { /* even index: a(i) = a(i-1) + prime(i+1) */
                val = cache[i-2] + (seq_t)p;
            } else { /* odd index (i ≥ 3): a(i) = a(i-1) * prime(i+1) */
                val = cache[i-2] * (seq_t)p;
            }
        }
        cache[i - 1] = val;
    }
    if (computed < n) computed = n;
    return cache[n - 1];
}
