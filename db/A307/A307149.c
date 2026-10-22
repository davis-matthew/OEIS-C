/*
 * A307149  Number of binary partitions of n in which exactly two of the
 *          powers of 2 are used an odd number of times.
 * https://oeis.org/A307149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{0 ≤ i < j, 2^i+2^j≤n, n-2^i-2^j even} p((n-2^i-2^j)/2)
 *            where p(m) is the number of binary partitions of m
 * Range:     u8 n <= 49, u16 n <= 199, u32 n <= 1443, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *p_cache = NULL;   /* caches values of p(m) */
static idx_t  p_size  = 0;      /* number of elements in p_cache */

/* compute binary-partition numbers up to max_index (inclusive) */
static void ensure_p_cache(idx_t max_index) {
    if (p_size > max_index) return;          /* already enough */
    idx_t new_size = max_index + 1;
    seq_t *new_cache = (seq_t*)realloc(p_cache, new_size * sizeof(seq_t));
    if (!new_cache && new_size > 0) abort();  /* allocation failed */
    p_cache = new_cache;

    /* initialise and compute DP */
    for (idx_t i = 0; i < new_size; ++i) p_cache[i] = 0;
    p_cache[0] = 1;                            /* empty partition */

    for (idx_t power = 1; power < new_size; power <<= 1) {
        for (idx_t s = power; s < new_size; ++s) {
            p_cache[s] += p_cache[s - power];
        }
    }
    p_size = new_size;
}

seq_t a(idx_t n) {
    /* we need p(m) for m up to ⌊n/2⌋ */
    ensure_p_cache(n >> 1);

    seq_t sum = 0;
    for (idx_t pi = 1; pi <= n; pi <<= 1) {
        for (idx_t pj = pi << 1; pj <= n - pi; pj <<= 1) {
            idx_t t = n - pi - pj;
            if (t & 1) continue;          /* parity must be even */
            sum += p_cache[t >> 1];
        }
    }
    return sum;
}
