/*
 * A321774  Number of compositions of n into parts with distinct
 *          multiplicities and with exactly four parts.
 * https://oeis.org/A321774
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n‑3) + a(n‑4) – a(n‑7) for n>10 Initial values (n<11):
 *            a(4)=1, a(5)=4, a(6)=4, a(7)=8, a(8)=5, a(9)=8, a(10)=12 n<4 is
 *            undefined (return 0)
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache contains values up to n. */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;          /* indices 0..n inclusive */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;                 /* allocation failed, unlikely */
        /* initialize new entries */
        for (size_t i = (size_t)cache_size; i < new_size; ++i) {
            if (i < 4) {
                new_cache[i] = 0;
            } else if (i == 4) {
                new_cache[i] = (seq_t)1;
            } else if (i == 5) {
                new_cache[i] = (seq_t)4;
            } else if (i == 6) {
                new_cache[i] = (seq_t)4;
            } else if (i == 7) {
                new_cache[i] = (seq_t)8;
            } else if (i == 8) {
                new_cache[i] = (seq_t)5;
            } else if (i == 9) {
                new_cache[i] = (seq_t)8;
            } else if (i == 10) {
                new_cache[i] = (seq_t)12;
            } else {
                /* recurrence: a(i) = a(i-3)+a(i-4)-a(i-7) */
                new_cache[i] = new_cache[i-3] + new_cache[i-4] - new_cache[i-7];
            }
        }
        cache = new_cache;
        cache_size = new_size;
    }

    if (n >= cache_size) return 0;  /* safety net */
    return cache[n];
}
