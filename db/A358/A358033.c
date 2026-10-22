/*
 * A358033  a(1) = 2; a(n) - a(n-1) = A093803(a(n-1)), the largest odd proper
 *          divisor of a(n-1).
 * https://oeis.org/A358033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2; For n ≥ 7: a(n) = 5 * a(n-6) (for n = 7 add +2: a(7) =
 *            5*a(1)+2) The first six terms are pre‑computed: a(1)=2, a(2)=3,
 *            a(3)=4, a(4)=5, a(5)=6, a(6)=9. This follows from the generating
 *            function x*(2+3x+4x²+5x³+6x⁴+9x⁵+2x⁶)/(1‑5x⁶)
 * Range:     u8 n <= 18, u16 n <= 39, u32 n <= 80, u64 n <= 163,
 *            u128 n <= 329, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for previously computed terms.
   cache[0] stores a(1), cache[1] stores a(2), etc. */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure the cache can hold at least `need` elements. */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    if (new_cap < (size_t)need) new_cap = need;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf) return;          /* allocation failed – keep old cache */
    cache = new_buf;
    cache_cap = new_cap;
}

/* Precomputed values for n = 1 … 6. */
static const seq_t init_vals[6] = {2, 3, 4, 5, 6, 9};

/* Return the n‑th term of OEIS A358033. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; out of range */

    /* Compute all terms up to n if not already cached. */
    if (n > cache_len) {
        ensure_cache_cap(n);
        for (idx_t i = cache_len; i < n; ++i) {
            idx_t term = i + 1;          /* convert 0‑based index to n */
            seq_t val;

            if (term <= 6) {
                /* Use the table of first six terms. */
                val = init_vals[term - 1];
            } else if (term == 7) {
                /* Special adjustment for term 7. */
                val = cache[0] * ((seq_t)5) + (seq_t)2;
            } else {
                /* For n ≥ 8 :  a(n) = 5 * a(n‑6). */
                val = cache[i - 6] * ((seq_t)5);
            }

            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
