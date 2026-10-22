/*
 * A301995  Number of nX4 0..1 arrays with every element equal to 1, 2 or 4
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3); Initial values: a(1)=2,
 *            a(2)=8, a(3)=26
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Grow a sequence array to hold at least 'cap' elements.
   The array is a simple resizable buffer; if size has to increase,
   a new buffer is allocated and the old contents are copied. */
static void grow_seq(seq_t **arr, size_t *cap, size_t needed) {
    if (*cap >= needed) return;
    size_t new_cap = (*cap > 0) ? (*cap * 2) : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)malloc(sizeof(seq_t) * new_cap);
    for (size_t i = 0; i < *cap; ++i) new_arr[i] = (*arr)[i];
    free(*arr);
    *arr = new_arr;
    *cap = new_cap;
}

seq_t a(idx_t n) {
    /* The sequence is defined for positive indices only. */
    if (n < 1) return 0;

    /* Static cache to store computed values up to the maximum n seen. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* largest index stored (1‑based) */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* Ensure we have storage for at least 'n' elements. */
    grow_seq(&cache, &cache_cap, (size_t)n + 1);

    /* Initialize base values if not already done. */
    if (cache_len < 1) {
        cache[1] = (seq_t)2;
        cache_len = 1;
    }
    if (cache_len < 2) {
        cache[2] = (seq_t)8;
        cache_len = 2;
    }
    if (cache_len < 3) {
        cache[3] = (seq_t)26;
        cache_len = 3;
    }

    /* If we already have the requested value, just return it. */
    if (n <= (idx_t)cache_len) return cache[(size_t)n];

    /* Compute values up to n using the recurrence. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        /* a(i) = 4*a(i-1) - 4*a(i-2) + a(i-3) */
        seq_t term1 = 4 * cache[i - 1];
        seq_t term2 = 4 * cache[i - 2];
        seq_t term3 = cache[i - 3];
        cache[i] = term1 - term2 + term3;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n];
}
