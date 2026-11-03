/*
 * A241497  q-Pell numbers with q=2.
 * https://oeis.org/A241497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 2^(n-2)*a(n-2), a(0)=0, a(1)=1 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    if (cache == NULL) {
        cache = mem_new(seq_t, 2);
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    if (n > max_computed) {
        cache = mem_resize(seq_t, cache, (n + 1));
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t term1 = ((seq_t)2) * cache[i - 1];
            seq_t pow_val = pow2(i - 2);
            seq_t term2 = pow_val * cache[i - 2];
            cache[i] = term1 + term2;
        }
        max_computed = n;
    }

    return cache[n];
}
