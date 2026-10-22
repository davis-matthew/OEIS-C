/*
 * A396769  Value of the Sackin index for the rooted binary echelon tree with
 *          n leaves.
 * https://oeis.org/A396769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003071(n) + n - 1 Recurrence used: Let k =
 *            floor(log2(n)); p = 2^k if n == p : a(n) = k * p else : a(n) =
 *            a(n-p) + n + k * p a(1) = 0
 * Range:     u8 n <= 45, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* floor_log2_uint64 for 64‑bit unsigned values, defined for n > 0 */
static inline idx_t floor_log2_uint64(idx_t v)
{
    /* __builtin_clzll returns count of leading zeros; valid for n>0 */
    return 63 - __builtin_clzll(v);
}

/* static cache for memoisation */
static seq_t *sackin_cache = NULL;
static idx_t sackin_cap   = 0;
static idx_t sackin_last  = 0;

/* main function required by framework */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* safety, offset is 1 */

    /* grow cache if needed */
    if (n > sackin_cap) {
        idx_t new_cap = sackin_cap ? sackin_cap << 1 : 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)malloc((new_cap + 1) * sizeof(seq_t));
        if (!new_cache) return 0;            /* allocation failed */

        /* copy existing entries, keep index 0 defined as 0 */
        new_cache[0] = 0;
        for (idx_t i = 1; i <= sackin_cap; ++i) new_cache[i] = sackin_cache[i];
        free(sackin_cache);
        sackin_cache = new_cache;
        sackin_cap   = new_cap;
    }

    /* fill cache up to n */
    for (idx_t i = sackin_last + 1; i <= n; ++i) {
        idx_t k = floor_log2_uint64(i);
        idx_t p = ((idx_t)1) << k;          /* largest power of 2 <= i */

        if (i == p) {                      /* i is a power of 2 */
            sackin_cache[i] = (seq_t)k * (seq_t)p;
        } else {
            sackin_cache[i] = sackin_cache[i - p]
                              + (seq_t)i
                              + (seq_t)k * (seq_t)p;
        }
    }

    sackin_last = n;
    return sackin_cache[n];
}
