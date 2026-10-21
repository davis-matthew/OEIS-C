/*
 * A244310  a(n) = L(n)^3 - L(n)^2, where L(n) is the n-th Lucas number
 *          (A000032).
 * https://oeis.org/A244310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L(n)^3 - L(n)^2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Use static cache to store computed Lucas numbers */
static seq_t *lucas_cache = NULL;      /* array of Lucas numbers L(0)..L(m) */
static idx_t lucas_cache_len = 0;      /* current number of stored values */
static idx_t lucas_cache_cap = 0;      /* allocated capacity */

/* Compute A244310. */
seq_t a(idx_t n)
{
    /* Ensure cache is initialized. */
    if (lucas_cache == NULL) {
        lucas_cache_cap = 2;            /* start with capacity for L0 and L1 */
        lucas_cache_len = 2;
        lucas_cache = (seq_t *)malloc(lucas_cache_cap * sizeof(seq_t));
        if (lucas_cache == NULL) return 0;   /* allocation failure (unlikely) */
        lucas_cache[0] = (seq_t)2; /* L(0) */
        lucas_cache[1] = (seq_t)1; /* L(1) */
    }

    /* Grow the cache if needed and compute missing values. */
    while (n >= lucas_cache_len) {
        /* Resize if necessary. */
        if (lucas_cache_len == lucas_cache_cap) {
            idx_t new_cap = lucas_cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(lucas_cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0; /* allocation failure */
            lucas_cache = tmp;
            lucas_cache_cap = new_cap;
        }
        /* Compute next Lucas number: L(k) = L(k-1) + L(k-2) */
        seq_t next = lucas_cache[lucas_cache_len - 1] + lucas_cache[lucas_cache_len - 2];
        lucas_cache[lucas_cache_len++] = next;
    }

    seq_t L = lucas_cache[n];
    /* a(n) = L^3 - L^2 = L^2 * (L - 1) */
    seq_t result = L * L * (L - (seq_t)1);
    return result;
}
