/*
 * A311566  Coordination sequence Gal.4.93.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the 18‑term linear recurrence * a(n) = -2a(n-1)
 *            -3a(n-2) -3a(n-3) -3a(n-4) -a(n-5) +a(n-6) * +4a(n-7) +5a(n-8)
 *            +6a(n-9) +5a(n-10)+4a(n-11)+a(n-12) * -a(n-13)
 *            -3a(n-14)-3a(n-15)-3a(n-16)-2a(n-17)-a(n-18) * for n > 18, with
 *            initial terms a(0)..a(18) given below
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"

static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;

static const seq_t init_terms[19] = {
    1, 4, 8, 12, 18, 21, 25, 31, 32, 39,
    42, 45, 53, 54, 61, 62, 66, 74, 74
};

static void init_cache(void)
{
    ensure_capacity(19, &seq_cap, (void **)&seq_cache, sizeof(seq_t));
    for (size_t i = 0; i < 19; ++i)
        seq_cache[i] = init_terms[i];
}

seq_t a(idx_t n)
{
    if (n < 19) {
        if (seq_cache == NULL) init_cache();
        return seq_cache[n];
    }
    if (seq_cache == NULL) {
        init_cache();
        /* ensure cache has at least n+1 entries */
    }
    if ((size_t)n >= seq_cap) {
        ensure_capacity((size_t)n + 1, &seq_cap, (void **)&seq_cache, sizeof(seq_t));
    }
    /* Compute missing terms up to n */
    for (size_t i = 19; i <= (size_t)n; ++i) {
        seq_t pos = seq_cache[i-6]
                  + 4 * seq_cache[i-7]
                  + 5 * seq_cache[i-8]
                  + 6 * seq_cache[i-9]
                  + 5 * seq_cache[i-10]
                  + 4 * seq_cache[i-11]
                  + seq_cache[i-12];
        seq_t neg = 2 * seq_cache[i-1]
                  + 3 * seq_cache[i-2]
                  + 3 * seq_cache[i-3]
                  + 3 * seq_cache[i-4]
                  + seq_cache[i-5]
                  + seq_cache[i-13]
                  + 3 * seq_cache[i-14]
                  + 3 * seq_cache[i-15]
                  + 3 * seq_cache[i-16]
                  + 2 * seq_cache[i-17]
                  + seq_cache[i-18];
        seq_cache[i] = pos - neg;  /* pos >= neg for all n>=19 */
    }
    return seq_cache[n];
}
