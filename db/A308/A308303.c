/*
 * A308303  Number of integer-sided triangles with perimeter n and at least
 *          one even side length.
 * https://oeis.org/A308303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of integer‐sided triangles with perimeter n and at least
 *            one even side length (OEIS A308303)
 * Range:     u8 n <= 111, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static bool   *computed = NULL;
    static idx_t  cache_size = 0;

    if (n == 0) return 0;                       /* offset starts at 1 */

    /* Grow cache if needed */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        bool  *new_comp  = (bool  *)realloc(computed, new_size * sizeof(bool));
        if (!new_cache || !new_comp) return 0; /* memory failure */

        for (idx_t i = cache_size; i < new_size; ++i) {
            new_cache[i] = 0;
            new_comp[i]  = false;
        }
        cache = new_cache;
        computed = new_comp;
        cache_size = new_size;
    }

    if (computed[n]) return cache[n];

    seq_t total = 0;
    idx_t n_val = n;

    /* Count all integer triangles with perimeter n */
    idx_t c_start = (n_val + 2) / 3;   /* ceil(n/3) */
    idx_t c_end   = (n_val - 1) / 2;   /* floor((n-1)/2) */

    for (idx_t c = c_start; c <= c_end; ++c) {
        idx_t sum = n_val - c;
        idx_t upper_a = sum / 2;                               /* floor((n-c)/2) */
        idx_t lower_a = (n_val > 2 * c) ? n_val - 2 * c : 1;    /* max(1, n-2c) */
        if (lower_a > upper_a) continue;
        total += (seq_t)(upper_a - lower_a + 1);
    }

    /* Subtract triangles whose all sides are odd (only possible when n is odd) */
    if (n_val & 1) {
        for (idx_t c = c_start; c <= c_end; ++c) {
            if (!(c & 1)) continue;   /* only odd c produce all‑odd triangles */
            idx_t sum = n_val - c;
            idx_t upper_a = sum / 2;
            idx_t lower_a = (n_val > 2 * c) ? n_val - 2 * c : 1;
            if (lower_a > upper_a) continue;
            idx_t odd_cnt = (upper_a + 1) / 2 - lower_a / 2; /* odds in [lower_a,upper_a] */
            total -= (seq_t)odd_cnt;
        }
    }

    cache[n] = total;
    computed[n] = true;
    return total;
}
