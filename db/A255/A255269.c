/*
 * A255269  a(n) = Product_{k=1..n} k!^k.
 * https://oeis.org/A255269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} k!^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_one.h"

static seq_t *cache = NULL;      /* cached a(k) for k >= 1 */
static size_t cache_len = 0;     /* number of cached elements */
static seq_t current_fact = 1;   /* (k-1)! for the next iteration */

seq_t a(idx_t n) {
    if (n == 0) return seq_one();          /* undefined but harmless */

    /* Extend the cache if necessary */
    if (cache_len < (size_t)n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        seq_t prev_val = (cache_len > 0) ? cache[cache_len - 1] : seq_one();

        for (idx_t k = cache_len + 1; k <= n; ++k) {
            /* Update factorial: current_fact = k!  */
            current_fact = seq_mul(current_fact, (seq_t)k);

            /* Compute (k!)^k */
            seq_t power_k = seq_pow(current_fact, k);

            /* a(k) = a(k-1) * (k!)^k */
            seq_t new_val = seq_mul(prev_val, power_k);

            cache[k - 1] = new_val;
            prev_val = new_val;
        }

        cache_len = (size_t)n;
    }

    return cache[n - 1];
}
